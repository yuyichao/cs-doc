(defvar *drawing-method-hash* (make-hash-table))


(defstruct (graphics-device (:constructor %make-gdev)
                            (:conc-name gd-)
                            (:print-function print-gd))
  display
  screen
  black
  white
  font
  font-width
  font-ascent
  font-height
  x-min
  y-min
  x-max
  y-max
  border
  gc
  event-mask
  object-set
  window
  obj-list
  redrawer
  button-handler
  )
                            

(defun print-gd (gd stream depth)
  (format stream "#<GRAPHICS-DEVICE: ~A OBJECTS>" (length (gd-obj-list gd))))


(defun make-graphics-device (name width height)
  (require :clx)
  (let* ((display (ext:open-clx-display
                   (cdr (assoc :display ext:*environment-list*))))
         (screen (first (xlib:display-roots display)))
         (black (xlib:screen-black-pixel screen))
         (white (xlib:screen-white-pixel screen))
         (font (xlib:open-font display "8x13"))
         (font-width (xlib:char-width font 0))
         (font-ascent (xlib:font-ascent font))
         (font-height (+ font-ascent (xlib:font-descent font)))
         (gc (xlib:create-gcontext :drawable (xlib:screen-root screen)
                                   :font font :exposures nil
                                   :fill-style :solid :fill-rule :even-odd
                                   :foreground black :background white))
         (event-mask (xlib:make-event-mask :exposure
                                           :button-press 
                                           ))
         (object-set
          (system:make-object-set name #'ext:default-clx-event-handler))

         (window (xlib:create-window :parent (xlib:screen-root screen)
				     :x 350 :y 200
				     :width width
				     :height height
				     :border-width 5
				     :event-mask event-mask
                                     :background white))
         (new-gd (%make-gdev
                  :display display
                  :screen screen
                  :black black
                  :white white
                  :font font
                  :font-width font-width
                  :font-ascent font-ascent
                  :font-height font-height
                  :x-min 0
                  :y-min 0
                  :x-max 100
                  :y-max 100
                  :border 0.1
                  :GC gc
                  :event-mask event-mask
                  :object-set object-set
                  :redrawer #'default-redrawer
                  :window window)))

    (labels ((exposure-handler (obj event-key window x y width height count send)
                (declare (ignore obj event-key window count send))
                (funcall (gd-redrawer new-gd) new-gd x y width height))
             
             (no-exposure-handler (obj event-key window major minor send)
                (declare (ignore obj event-key window major minor send)) t)

             (client-message-handler (obj event-key &rest lst)
                (declare (ignore obj event-key))
                (xlib:unmap-window window)
                (xlib:display-force-output display))

             (button-press-handler (obj event-key &rest lst)
                (declare (ignore obj event-key))
                (if (gd-button-handler new-gd)
                    (funcall (gd-button-handler new-gd)
                             new-gd
                             (nth 10 lst) ;number
                             (nth 4 lst)  ;x
                             (nth 5 lst)))))  ;y

      (ext:serve-exposure object-set #'exposure-handler)
      (ext:serve-no-exposure object-set #'no-exposure-handler)
      (ext:serve-client-message  object-set #'client-message-handler)
      (ext:serve-button-press  object-set #'button-press-handler)

      (ext:enable-clx-event-handling display #'ext:object-set-event-handler)
      
      (setf (xlib:wm-name window) name)
      (xlib::set-wm-protocols window '(WM_DELETE_WINDOW))
      (system:add-xwindow-object window window object-set))

    (xlib:map-window window)
    (xlib:display-force-output display)
    new-gd))

(defun graphics-set-coordinate-limits (gd x-min y-min x-max y-max)
  (cond ((or (<= x-max x-min) (<= y-max y-min))
         (print "Upper bound should be larger than lower.") nil)
        (t (setf (gd-x-min gd) x-min)
           (setf (gd-y-min gd) y-min)
           (setf (gd-x-max gd) x-max)
           (setf (gd-y-max gd) y-max)
           (graphics-redraw gd))))

(defun graphics-coordinate-limits (gd)
  (values (gd-x-min gd)
          (gd-y-min gd)
          (gd-x-max gd)
          (gd-y-max gd)))

(defun graphics-coord-map (gd)
  (lambda (x y)
    (values (funcall (graphics-coord-map-x gd) x)
            (funcall (graphics-coord-map-y gd) y))))

(defun graphics-coord-map-x (gd)
  (lambda (x)
    (let* ((x-min (gd-x-min gd))
           (x-max (gd-x-max gd))
           (width (graphics-device-width gd)))
      (floor (* width (/ (- x x-min) (- x-max x-min)))))))

(defun graphics-coord-map-y (gd)
  (lambda (y)
    (let* ((y-min (gd-y-min gd))
           (y-max (gd-y-max gd))
           (height (graphics-device-height gd)))
      (- height (floor (* height (/ (- y y-min) (- y-max y-min))))))))

(defun graphics-length-map-x (gd)
  (lambda (len)
    (let* ((x-min (gd-x-min gd))
           (x-max (gd-x-max gd))
           (width (graphics-device-width gd)))
      (floor (* width (/ len (- x-max x-min)))))))

(defun graphics-length-map-y (gd)
  (lambda (len)
    (let* ((y-min (gd-y-min gd))
           (y-max (gd-y-max gd))
           (height (graphics-device-height gd)))
      (floor (* height (/ len (- y-max y-min)))))))

; (funcall (graphics-coord-map gd1) 30 30)
; (funcall (graphics-length-map-x gd1) 30)

(defun default-redrawer (gd x y width height)

  (let* ((window (gd-window gd))
         (display (gd-display gd))
         (obj-list (gd-obj-list gd))
         (gc (gd-gc gd))
         (font (gd-font gd)))

  (unless (eq (xlib:window-map-state window) :viewable)
    (xlib:map-window window)
    (xlib:display-force-output display))

  ;; clear the whole window
  (xlib:clear-area window :x 0 :y 0 :width (xlib:drawable-width window)
		   :height (xlib:drawable-height window))
  (xlib:display-force-output display)

  ;; draw objects
  (dolist (obj obj-list)
    (let ((drawer (gethash (car obj) *drawing-method-hash*)))
      ;(print obj) (print drawer)
      (if drawer
          (apply drawer gd (cdr obj))
        (format t "I Don't know how to draw the type ~A" (car obj)))))

  (xlib:display-force-output display)))


; (setf (gd-redrawer gd1) #'default-redrawer)

; (gd-obj-list gd1)

(defun register-drawer (type proc)
  (setf (gethash type *drawing-method-hash*) proc))

(defmacro with-graphics-device (gd &body body)
  `(let* ((window (gd-window ,gd))
          (display (gd-display ,gd))
          (obj-list (gd-obj-list ,gd))
          (gc (gd-gc ,gd))
          (font (gd-font ,gd))
          (x-min (gd-x-min ,gd))
          (y-min (gd-y-min ,gd))
          (x-max (gd-x-max ,gd))
          (y-max (gd-y-max ,gd))
          (coord-map (graphics-coord-map ,gd))
          (coord-map-x (graphics-coord-map-x ,gd))
          (coord-map-y (graphics-coord-map-y ,gd))
          (length-map-x (graphics-length-map-x ,gd))
          (length-map-y (graphics-length-map-y ,gd))
          (width (graphics-device-width ,gd))
          (height (graphics-device-height ,gd)))
     ,@body))


(defun graphics-redraw (gd)
  (with-graphics-device gd
   (funcall (gd-redrawer gd) gd 0 0 width height)))

; (graphics-redraw gd1)


(defun graphics-flush (gd)
  (xlib:display-force-output (gd-display gd)))

  
(defun graphics-clear (gd)
  (with-graphics-device gd
     (setf (gd-obj-list gd) nil)
     (xlib:clear-area window :x 0 :y 0 :width (xlib:drawable-width window)
                      :height (xlib:drawable-height window))
     (xlib:display-force-output display)))



(defmacro define-drawer (drawer-name redrawer-name args &body body)
  (let ((type-tag (let ((type (cadr (memq '&type args))))
                    (if type (progn (setf args (remove-n '&type args 2))
                                    type)
                      (gensym))))
        (new-drawer (gensym))
        (obj (gensym))
        (gd (gensym))
        (n-args (length args)))


    `(let ((,new-drawer #'(lambda ,args ,@body)))

       (register-drawer ',type-tag ,new-drawer)

       (defun ,drawer-name (,gd ,@(cdr args))
         (let* ((,obj (list ',type-tag ,@(cdr args))))
                               
           (push ,obj (gd-obj-list ,gd))
           (apply ,new-drawer ,gd (list ,@(cdr args)))
           (graphics-flush ,gd)
           ,obj))
       (defun ,redrawer-name (,gd ,obj &rest lst)
         (cond ((null lst) 
                (setf (gd-obj-list ,gd)
                      (remove ,obj (gd-obj-list ,gd)))
                (graphics-redraw ,gd)
                ,obj)
               ((not (= (length lst) ,(- n-args 1)))
                (format t "~&~A : accept ~A args, get ~A~%"
                        ,(symbol-name redrawer-name) ,(+ n-args 1)  (+ (length lst) 2)))
               (t 
                (setf (cdr ,obj) lst)
                (if (not (memq ,obj (gd-obj-list ,gd)))
                    (push ,obj (gd-obj-list ,gd)))
                (graphics-redraw ,gd) ,obj))
         ))))



(define-drawer graphics-draw-dot graphics-redraw-dot
  (gd x y radius &type :dot)
  (with-graphics-device gd
    (xlib:draw-arc window gc 
                   (- (funcall coord-map-x x) radius)
                   (- (funcall coord-map-y y) radius)
                   (* 2 radius)
                   (* 2 radius)
                   0 (* 2 3.14159) t)))


(define-drawer graphics-draw-circle graphics-redraw-circle 
  (gd x y radius &type :circle)
  (with-graphics-device gd
    (xlib:draw-arc window gc 
                   (funcall coord-map-x (- x radius))
                   (funcall coord-map-y (+ y radius))
                   (funcall length-map-x (* 2 radius))
                   (funcall length-map-y (* 2 radius))
                   0 (* 2 3.14159))))

(define-drawer graphics-draw-line  graphics-redraw-line
  (gd x1 y1 x2 y2 &type :line)
  (with-graphics-device gd
    (xlib:draw-line window gc 
                    (funcall coord-map-x x1)
                    (funcall coord-map-y y1)
                    (funcall coord-map-x x2)
                    (funcall coord-map-y y2))))

(define-drawer graphics-draw-rect graphics-redraw-rect
  (gd x y w h &type :rect)
  (with-graphics-device gd
      (xlib:draw-rectangle window gc 
                           (funcall coord-map-x x) 
                           (funcall coord-map-y y) 
                           (funcall length-map-x w)
                           (funcall length-map-y h))))

(define-drawer graphics-draw-text graphics-redraw-text
  (gd x y text &type :text)
  (with-graphics-device gd
      (xlib:draw-glyphs window gc (funcall coord-map-x x)
                        (funcall coord-map-y y) text)))



(defun graphics-device-width (gd)
  (xlib:drawable-width (gd-window gd)))

(defun graphics-device-height (gd)
  (xlib:drawable-height (gd-window gd)))

; (graphics-device-width gd1)

(defun close-graphics-device (g)
  (xlib:unmap-window (gd-window g))
  (xlib:display-finish-output (gd-display g))
  (xlib:close-font (gd-font g))
  (xlib:free-gcontext (gd-gc g))
  (xlib:close-display (gd-display g)))




;; (setf gd1 (make-graphics-device "hello" 300 300))
;; (graphics-set-coordinate-limits gd1 0 0 200 200)
;; (graphics-draw-text gd1 50 20 "wang yin")
;; (graphics-draw-rect gd1 30 50 10 10)
;; (graphics-draw-line gd1 0 0 30 70)
;; (graphics-draw-line gd1 30 70 50 50)
;; (graphics-draw-circle gd1 50 50 20)

;; (loop for x = 1 then (* 1.3 x) 
;;       until (> x 200)
;;       do (graphics-draw-circle gd1 0 0 x))

;; (setf dot1 (graphics-draw-dot gd1 70 40 20))

;; (setf (fourth dot1) 40)

;; (graphics-redraw-dot gd1 dot1)

;; (setf wy (graphics-draw-text gd1 0 0 "rect. delaunay for 20 sites "))

;; (graphics-redraw-text gd1 wy 80 40 "kick")

;; (graphics-redraw-text gd1 wy 100000 1729741 "rect. delaunay for 20 sites ")

;; (graphics-clear gd1)

;; (funcall (graphics-length-map-x gd1) 5)

;; (graphics-flush gd1)

;; (cadr (gd-obj-list gd1))

;; (graphics-redraw-text gd1 (cadr (gd-obj-list gd1)))

