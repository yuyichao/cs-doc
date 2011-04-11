;;; -*- Mode: Lisp; Package: SDRAW -*-
;;;
;;; SDRAW - draws cons cell structures.
;;;
;;; From the book "Common Lisp:  A Gentle Introduction to
;;;      Symbolic Computation" by David S. Touretzky.  
;;; The Benjamin/Cummings Publishing Co., 1990.
;;;
;;; This version is for CMU Common Lisp with CLX support for X Windows.
;;; Revised to include support for circular structures.
;;;
;;; User-level routines:
;;;   (SDRAW obj)  - draws obj on the display
;;;   (SDRAW-LOOP) - puts the user in a read-eval-draw loop
;;;   (SCRAWL obj) - interactively crawl around obj
;;;
;;; Variables:
;;;   *SDRAW-PRINT-CIRCLE*    If bound, overrides *PRINT-CIRCLE*.
;;;   *SDRAW-LEADING-ARROW*   Initially NIL.  Set to T to get leading arrows.
;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; The parameters below are in units of characters (horizontal)
;;; and lines (vertical).  They apply to all versions of SDRAW,
;;; but their values may change if cons cells are being drawn as
;;; bit maps rather than as character sequences.

(defparameter *sdraw-display-width* 79.)
(defparameter *sdraw-display-height* 24.)
(defparameter *sdraw-horizontal-atom-cutoff* 79.)
(defparameter *sdraw-horizontal-cons-cutoff* 65.)

(defparameter *etc-string* "etc.")
(defparameter *etc-spacing* 4.)

(defparameter *inter-atom-h-spacing* 3.)
(defparameter *cons-atom-h-arrow-length* 9.)
(defparameter *inter-cons-v-arrow-length* 3.)
(defparameter *cons-v-arrow-offset-threshold* 2.)
(defparameter *cons-v-arrow-offset-value* 1.)
(defparameter *leading-arrow-length* 4)

(defparameter *sdraw-num-lines* 25)
(defparameter *sdraw-vertical-cutoff* 22.)

(defvar *sdraw-leading-arrow* nil)
(defvar *sdraw-print-circle*)
(defvar *sdraw-circular-switch*)
(defvar *circ-detected* nil)
(defvar *circ-label-counter* 0)
(defparameter *circ-hash-table* (make-hash-table :test #'eq :size 20))

(defvar *line-endings* (make-array *sdraw-num-lines*))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; SDRAW and subordinate definitions.

(defun sdraw (obj &aux (*circ-detected* nil))
  (let ((*sdraw-circular-switch*
	 (if (boundp '*sdraw-print-circle*) *sdraw-print-circle*
	     *print-circle*))
	(start-col (if *sdraw-leading-arrow* *leading-arrow-length* 0)))
    (init-struct1 start-col)
    (clrhash *circ-hash-table*)
    (let* ((first-layout (struct1 obj 0 start-col 0 nil))
	   (second-layout (when *circ-detected*
			    (init-struct1 start-col)
			    (struct1 obj 0 start-col 0 t))))
      (draw-structure (or second-layout first-layout))
      (values))))



(defun init-struct1 (start-col)
  (setf *circ-label-counter* 0)
  (fill *line-endings* most-negative-fixnum)
  (struct-record-position 0 (- start-col *inter-atom-h-spacing*)))

(defun never-seen? (obj)
  (null (gethash obj *circ-hash-table*)))

(defun seen-twice? (obj)
  (numberp (gethash obj *circ-hash-table*)))

(defun needs-label? (obj)
  (zerop (gethash obj *circ-hash-table*)))



(defun struct1 (obj row root-col adj second-pass)
  (cond ((>= row *sdraw-vertical-cutoff*) (struct-process-etc row root-col adj))
	((not second-pass)
	 (enter-in-hash-table obj)
	 (struct-first-pass obj row root-col adj))
	(t (struct-second-pass obj row root-col adj))))

(defun enter-in-hash-table (obj)
  (unless (or (not *sdraw-circular-switch*)
	      (numberp obj)
	      (and (symbolp obj) (symbol-package obj)))
    (cond ((never-seen? obj) (setf (gethash obj *circ-hash-table*) t))
	  (t (setf (gethash obj *circ-hash-table*) 0)
	     (setf *circ-detected* t)))))

(defun struct-first-pass (obj row root-col adj)
  (if (seen-twice? obj)
      (struct-process-circ-reference obj row root-col adj)
      (if (atom obj)
	  (struct-unlabeled-atom (format nil "~S" obj) row root-col adj)
	  (struct-unlabeled-cons obj row root-col adj nil))))

(defun struct-second-pass (obj row root-col adj)
  (cond ((not (seen-twice? obj))
	 (if (atom obj)
	     (struct-unlabeled-atom (format nil "~S" obj) row root-col adj)
	     (struct-unlabeled-cons obj row root-col adj t)))
	((needs-label? obj)
	 (if (atom obj)
	     (struct-label-atom obj row root-col adj)
	     (struct-label-cons obj row root-col adj)))
	(t (struct-process-circ-reference obj row root-col adj))))


;;; Handle the simplest case:  an atom or cons with no #n= label.

(defun struct-unlabeled-atom (atom-string row root-col adj)
  (let* ((start-col (struct-find-start row root-col adj))
	 (end-col (+ start-col adj (length atom-string))))
    (cond ((< end-col *sdraw-horizontal-atom-cutoff*)
	   (struct-record-position row end-col)
	   (list 'atom row (+ start-col adj) atom-string))
	  (t (struct-process-etc row root-col adj)))))

(defun struct-unlabeled-cons (obj row root-col adj second-pass)
  (let* ((cons-start (struct-find-start row root-col adj))
	 (car-structure
	  (struct1 (car obj)
		   (+ row *inter-cons-v-arrow-length*)
		   cons-start adj second-pass))
	 (start-col (third car-structure)))
    (if (>= start-col *sdraw-horizontal-cons-cutoff*)
	(struct-process-etc row root-col adj)
	(progn
	  (struct-record-position row (- (+ start-col
					    *cons-atom-h-arrow-length*)
					 adj *inter-atom-h-spacing*))
	  (list 'cons row start-col car-structure
		(struct1 (cdr obj) row (+ start-col *cons-atom-h-arrow-length*)
			 0 second-pass))))))

(defun struct-process-etc (row root-col adj)
  (let ((start-col (struct-find-start row root-col adj)))
    (struct-record-position
      row
      (+ start-col adj (length *etc-string*) *etc-spacing*))
    (list 'msg row (+ start-col adj) *etc-string*)))




;;; Handle objects that need to be labeled with #n=.
;;; Called only on the second pass.

(defun struct-label-atom (obj row root-col adj)
  (assign-label obj)
  (let* ((circ-string (format nil "#~S=" (gethash obj *circ-hash-table*)))
	 (newadj (struct-find-adj row root-col adj (length circ-string)))
	 (atom-string (format nil "~S" obj))
	 (start-col (struct-find-start row root-col adj))
	 (end-col (+ start-col newadj (length atom-string))))
    (cond ((< end-col *sdraw-horizontal-atom-cutoff*)
	   (struct-record-position row end-col)
	   (list 'atom row (+ start-col newadj) atom-string circ-string))
	  (t (struct-process-etc row root-col adj)))))

(defun struct-label-cons (obj row root-col adj)
  (assign-label obj)
  (let* ((string (format nil "#~S=" *circ-label-counter*))
	 (newadj (struct-find-adj row root-col adj (length string)))
	 (cons-start (struct-find-start row root-col adj))
	 (car-structure
	  (struct1 (car obj)
		   (+ row *inter-cons-v-arrow-length*)
		   cons-start newadj t))
	 (start-col (third car-structure)))
    (if (>= start-col *sdraw-horizontal-cons-cutoff*)
	(struct-process-etc row root-col adj)
	(progn
	  (struct-record-position row (- (+ start-col
					    *cons-atom-h-arrow-length*)
					 adj *inter-atom-h-spacing*))
	  (list 'cons row start-col car-structure
		(struct1 (cdr obj) row
			 (+ start-col *cons-atom-h-arrow-length*) 0 t)
		string)))))

(defun assign-label (obj)
  (setf (gethash obj *circ-hash-table*)
	(incf *circ-label-counter*)))


;;; Handle circular references by displaying them as #n#.
;;; When called on the first pass, this function always uses a label of 0.
;;; It will get the label right on the second pass.

(defun struct-process-circ-reference (obj row root-col adj)
  (let ((start-col (struct-find-start row root-col adj))
	(string (format nil "#~S#" (gethash obj *circ-hash-table*))))
    (struct-record-position
      row
      (+ (+ start-col adj) (length string)))
    (list 'msg row (+ start-col adj) string)))



;;; Support functions.

(defun struct-find-start (row root-col adj)
  (max root-col
       (- (+ *inter-atom-h-spacing* (aref *line-endings* row)) adj)))

(defun struct-find-adj (row col adj size)
  (let* ((line-end (max 0 (+ *inter-atom-h-spacing*
			     (aref *line-endings* row))))
	 (newadj (- line-end (- col (max size adj)))))
    (max adj (min (max newadj 0) size))))

(defun struct-record-position (row end-col)
  (setf (aref *line-endings* row) end-col))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; SDRAW-LOOP and subordinate definitions.

(defparameter *sdraw-loop-prompt-string* "S> ")

(defun sdraw-loop ()
  "Read-eval-print loop using sdraw to display results."
  (format t "~&Type any Lisp expression, or (ABORT) to exit.~%~%")
  (sdl1))

(defun sdl1 ()
  (loop
    (format t "~&~A" *sdraw-loop-prompt-string*)
    (force-output t)
    (let ((form (read)))
      (setf +++ ++
	    ++  +
	    +   -
	    -   form)
      (let ((result (multiple-value-list
		     (handler-case (eval form)
		       (error (condx) condx)))))
	(typecase (first result)
	  (error (display-sdl-error result))
	  (t (setf /// //
		   //  /
		   /   result
		   *** **
		   **  *
		   *   (first result))
	     (display-sdl-result *)))))))

(defun display-sdl-result (result)
  (sdraw result)
  (let* ((*print-circle* (if (boundp '*sdraw-print-circle*)
			     *sdraw-print-circle*
		             *print-circle*))
	 (*print-length* nil)
	 (*print-level* nil)
	 (*print-pretty* #+cmu t #-cmu nil)
	 (full-text (format nil "Result:  ~S" result))
	 (text (if (> (length full-text)
		      *sdraw-display-width*)
		   (concatenate 'string
		     (subseq full-text 0 (- *sdraw-display-width* 4))
		     "...)")
		   full-text)))
    (if (consp result)
        (format t "~%~A~%" text))
    (terpri)))

(defun display-sdl-error (error)
  (format t "~A~%~%" error))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; SCRAWL and subordinate definitions.

(defparameter *scrawl-prompt-string* "SCRAWL> ")
(defvar *scrawl-object* nil)
(defvar *scrawl-current-obj* nil)
(defvar *extracting-sequence* nil)

(defun scrawl (obj)
  "Read-eval-print loop to travel through list"
  (format t "~&Crawl through list:  'H' for help, 'Q' to quit.~%~%")
  (setf *scrawl-object* obj)
  (scrawl-start-cmd)
  (scrawl1))

(defun scrawl1 ()
  (loop
    (format t "~&~A" *scrawl-prompt-string*)
    (let ((command (read-uppercase-char)))
      (case command
	(#\A (scrawl-car-cmd))
	(#\D (scrawl-cdr-cmd))
	(#\B (scrawl-back-up-cmd))
	(#\S (scrawl-start-cmd))
	(#\H (display-scrawl-help))
	(#\Q (return))
	(t (display-scrawl-error))))))

(defun scrawl-car-cmd ()
  (cond ((consp *scrawl-current-obj*)
	 (push 'car *extracting-sequence*)
	 (setf *scrawl-current-obj* (car *scrawl-current-obj*)))
	(t (format t
	     "~&Can't take CAR or CDR of an atom.  Use B to back up.~%")))
  (display-scrawl-result))

(defun scrawl-cdr-cmd ()
  (cond ((consp *scrawl-current-obj*)
	 (push 'cdr *extracting-sequence*)
	 (setf *scrawl-current-obj* (cdr *scrawl-current-obj*)))
	(t (format t
	     "~&Can't take CAR or CDR of an atom.  Use B to back up.~%")))
  (display-scrawl-result))

(defun scrawl-back-up-cmd ()
  (cond (*extracting-sequence*
	 (pop *extracting-sequence*)
	 (setf *scrawl-current-obj*
	       (extract-obj *extracting-sequence* *scrawl-object*)))
	(t (format t "~&Already at beginning of object.")))
  (display-scrawl-result))

(defun scrawl-start-cmd ()
  (setf *scrawl-current-obj* *scrawl-object*)
  (setf *extracting-sequence* nil)
  (display-scrawl-result))

(defun extract-obj (seq obj)
  (reduce #'funcall
	  seq
	  :initial-value obj
	  :from-end t))

; (extract-obj '(car cdr cdr) '(1 2 3 4 5 6 7 8))


(defun get-car/cdr-string ()
  (if (null *extracting-sequence*)
      (format nil "'~S" *scrawl-object*)
      (format nil "(c~Ar '~S)"
	      (map 'string #'(lambda (x)
			       (ecase x
				 (car #\a)
				 (cdr #\d)))
		   *extracting-sequence*)
	      *scrawl-object*)))

(defun display-scrawl-result (&aux (*print-length* nil)
				   (*print-level* nil)
				   (*print-pretty* #+cmu t #-cmu nil)
				   (*print-circle* t))
  (let* ((extract-string (get-car/cdr-string))
	 (text (if (> (length extract-string) *sdraw-display-width*)
		   (concatenate 'string
		    (subseq extract-string 0
			    (- *sdraw-display-width* 4))
		    "...)")
		   extract-string)))
    (sdraw *scrawl-current-obj*)
    (format t "~&~%~A~%~%" text)))

(defun display-scrawl-help ()
  (format t "~&Legal commands:  A)car   D)cdr  B)back up~%")
  (format t "~&                 S)start Q)quit H)help~%"))

(defun display-scrawl-error ()
  (format t "~&Illegal command.~%")
  (display-scrawl-help))

(defun read-uppercase-char ()
  (let ((response (read-line)))
    (and (plusp (length response))
	 (char-upcase (char response 0)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; X11 constants and event handlers.
;;;

(require :clx)
(defvar *DISPLAY* (ext:open-clx-display
		   (cdr (assoc :display ext:*environment-list*))))
(defvar *SCREEN* (first (xlib:display-roots *display*)))
(defvar *BLACK* (xlib:screen-black-pixel *screen*))
(defvar *WHITE* (xlib:screen-white-pixel *screen*))
(defvar *FONT* (xlib:open-font *display* "8x13"))
(defvar *FONT-WIDTH* (xlib:char-width *font* 0) "works for fixed-size fonts")
(defvar *FONT-ASCENT* (xlib:font-ascent *font*))
(defvar *FONT-HEIGHT* (+ *font-ascent* (xlib:font-descent *font*)))
(defvar *GC* (xlib:create-gcontext :drawable (xlib:screen-root *screen*)
				   :font *font* :exposures nil
				   :fill-style :solid :fill-rule :even-odd
				   :foreground *black* :background *white*))

;;; X11 Event Handling (exposure events)
(defvar *x-object-set*
  (system:make-object-set "SDraw Window" #'ext:default-clx-event-handler))

(defvar *events* (xlib:make-event-mask :exposure
                                       :button-press 
                                       :resize-redirect))

(defun exposure-handler (obj event-key window x y width height count send)
  (declare (ignore obj event-key window x y width height count send))
  (do-redraw))

(defun no-exposure-handler (obj event-key window major minor send)
  (declare (ignore obj event-key window major minor send)) t)

(defun button-press-handler (obj event-key &rest lst)
  (declare (ignore obj event-key))
  (if *scrawl-object*
      (case (nth 10 lst)
        (1 (scrawl-car-cmd))
        (2 (scrawl-back-up-cmd))
        (3 (scrawl-cdr-cmd)))))

;; (defun resize-request-handler (obj event-key &rest lst)
;;   (declare (ignore obj event-key))
;;   (do-redraw))

(defun client-message-handler (obj event-key &rest lst)
  (declare (ignore obj event-key))
  (hide-window))

(ext:serve-exposure *x-object-set* #'exposure-handler)
(ext:serve-no-exposure *x-object-set* #'no-exposure-handler)
(ext:serve-client-message  *x-object-set* #'client-message-handler)
(ext:serve-button-press  *x-object-set* #'button-press-handler)

;; (ext:serve-resize-request  *x-object-set* #'resize-request-handler)

(defun enable-X11-handler ()
  (ext:enable-clx-event-handling *display* #'ext:object-set-event-handler))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;;  X11 Window: window-row and window-col translate char coordinates into
;;;  pixel coordinates.  init-window creates the X11 window and starts up the
;;;  event handler.  do-redraw is the function called when a redraw event
;;;  is requested, it handles resizing also.

(defun window-row (row &optional (offset 0)) (+ (* row *font-height*) offset))
(defun window-col (col &optional (offset 0)) (+ (* col *font-width*) offset))

(defvar *WINDOW-H-OFFSET* (* 2 *font-width*) "horizontal offset")
(defvar *WINDOW-V-OFFSET* (* 2 *font-height*) "vertical offset")
(defvar *WINDOW-WIDTH* (window-col *sdraw-display-width*
				   (* 2 *window-h-offset*)))
(defvar *WINDOW-HEIGHT* (window-row *sdraw-display-height*
				    (* 2 *window-v-offset*)))
(defvar *WINDOW* (xlib:create-window :parent (xlib:screen-root *screen*)
				     :x 350 :y 200
				     :width *window-width*
				     :height *window-height*
				     :border-width 5
				     :event-mask *events*
				     :background *white*))
(defun init-window ()
  (enable-X11-handler)
  (setf (xlib:wm-name *window*) "SDraw")
  (xlib::set-wm-protocols *window* '(WM_DELETE_WINDOW))
  (system:add-xwindow-object *window* *window* *x-object-set*))

(init-window)

(defun hide-window ()
  (xlib:unmap-window *window*)
  (xlib:display-force-output *display*))

(defvar *old-window-height* (xlib:drawable-height *window*))
(defvar *old-window-width* (xlib:drawable-width *window*))

(defun do-redraw ()
  (let ((h (xlib:drawable-height *window*))
	(w (xlib:drawable-width *window*)))

    (unless (and (eq h *old-window-height*) ; check for resize
		 (eq w *old-window-width*))
      (setf *old-window-height* h)
      (setf *old-window-width* w)
      (decf h (* 2 *window-h-offset*))
      (decf w (* 2 *window-v-offset*))
      (setf *sdraw-display-width* (floor w *font-width*))
      (setf *sdraw-display-height* (floor h *font-height*))
      (setf *sdraw-vertical-cutoff* (- *sdraw-display-height* 3))
      (setf *sdraw-horizontal-atom-cutoff* (1- *sdraw-display-width*))
      (setf *sdraw-horizontal-cons-cutoff* (- *sdraw-display-width* 15))
      (setf *line-endings*
	    (make-array *sdraw-display-height* :initial-element most-negative-fixnum)))
    (dump-display)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; The following definitions are specific to the X11 implementation.

(defvar *cons-width* 40)
(defvar *cons-height* 15)
(defvar *arrowhead-from-point* 8)
(defvar *arrowhead-from-axis* 5)

(defvar *obj-list* nil)

(defun arrowhead (x y type)
  (ecase type
    (:horiz
     `((:line ,x ,y ,(- x *arrowhead-from-point*)
	      ,(- y *arrowhead-from-axis*))
       (:line ,x ,y ,(- x *arrowhead-from-point*)
	      ,(+ y *arrowhead-from-axis*))))
    (:vert
     `((:line ,x ,y ,(- x *arrowhead-from-axis*)
	      ,(- y *arrowhead-from-point*))
       (:line ,x ,y ,(+ x *arrowhead-from-axis*)
	      ,(- y *arrowhead-from-point*))))))

(defun draw-structure (directions)
  (setf *obj-list* nil)
  (follow-directions directions)
  (dump-display))

(defun follow-directions (dirs &optional is-car)
  (ecase (car dirs)
    (cons (draw-cons dirs))
    ((atom msg) (draw-msg dirs is-car))))

(defun draw-cons (obj)
  (let* ((row (1- (window-row (second obj) *window-v-offset*)))
	 (col (window-col (third obj) *window-h-offset*))
	 (car-component (fourth obj))
	 (cdr-component (fifth obj))
	 (string (sixth obj))
	 (h-arrow-start-x (floor (+ col (* 0.75 *cons-width*))))
	 (v-arrow-start-x (floor (+ col (* 0.25 *cons-width*))))
	 (arrow-start-y (floor (+ row (* 0.5 *cons-height*))))
	 (circle-offset (floor (* 0.08 *cons-width*)))
	 (circle-y (- arrow-start-y circle-offset))
	 (v-arrowhead-row (+ row (* *font-height* 3)))
	 (h-arrowhead-col (1- (window-col (third cdr-component)
					  *window-h-offset*)))
	 (cdr-string? (if (eq 'cons (first cdr-component))
			  (sixth cdr-component)
			  (fifth cdr-component))))
    (if cdr-string? (decf h-arrowhead-col (length cdr-string?)))
    (when string
      (push `(:text ,(- col (window-col (length string)))
		    ,(+ row *font-ascent*) ,string)
	    *obj-list*))
    (push `(:rect ,col ,row ,*cons-width* ,*cons-height*) *obj-list*)
    (push `(:circle ,(- v-arrow-start-x circle-offset) ,circle-y) *obj-list*)
    (push `(:circle ,(- h-arrow-start-x circle-offset) ,circle-y) *obj-list*)
    (push `(:line ,(+ col (floor *cons-width* 2)) ,row
		  ,(+ col (floor *cons-width* 2)) ,(+ row *cons-height*))
	  *obj-list*)
    (push `(:line ,h-arrow-start-x ,arrow-start-y
		  ,h-arrowhead-col ,arrow-start-y) *obj-list*)
    (setf *obj-list* (append (arrowhead h-arrowhead-col arrow-start-y :horiz)
			     *obj-list*))
    (push `(:line ,v-arrow-start-x ,arrow-start-y
		  ,v-arrow-start-x ,v-arrowhead-row) *obj-list*)
    (setf *obj-list* (append (arrowhead v-arrow-start-x v-arrowhead-row :vert)
			     *obj-list*))
    (follow-directions car-component t)
    (follow-directions cdr-component)))

(defun draw-msg (obj is-car)
  (let* ((row (second obj))
	 (col (third obj))
	 (string (fourth obj))
	 (circ-string (fifth obj)))
    (when circ-string
      (setf string (concatenate 'string circ-string string))
      (decf col (length circ-string)))
    (push `(:text ,(+ (window-col col *window-h-offset*)
		      (if (and is-car (<= (length string)
					  *cons-v-arrow-offset-threshold*))
			  (* *cons-v-arrow-offset-value* *font-width*)
			  (if is-car 0 5)))
		  ,(+ *font-ascent* (window-row row *window-v-offset*))
		  ,string) *obj-list*)))

(defconstant *diameter* 7)
(defun create-circle ()
  (let ((pix (xlib:create-pixmap :width *diameter* :height *diameter*
				 :depth (xlib:drawable-depth *window*) 
                                 :drawable *window*))
	(data '((2 4)(1 5)(0 6)(0 6)(0 6)(1 5)(2 4))))
    (xlib:with-gcontext (*gc* :foreground *white*)
      (xlib:draw-rectangle pix *gc* 0 0 *diameter* *diameter* t))
    (do* ((line data (cdr line))
	  (x-data (car line) (car line))
	  (y 0 (1+ y)))
	 ((null line) pix)
      (xlib:draw-line pix *gc* (first x-data) y (second x-data) y))))

(defvar *circle* (create-circle))

;; (push `(:line 30 30 60 40) *obj-list*) (setf *obj-list* nil)
;; (push `(:circle 60 30) *obj-list*)
;; (xlib:copy-area *circle* *gc* 0 0 *diameter* *diameter*
;;                 *window* 30 30)

;; (xlib:draw-line *window* *gc* 30 30 60 40)
;; (xlib:display-force-output *display*)


(defun dump-display ()
  (unless (eq (xlib:window-map-state *window*) :viewable)
    (xlib:map-window *window*)
    (xlib:display-force-output *display*))
  (xlib:clear-area *window* :x 0 :y 0 :width (xlib:drawable-width *window*)
		   :height (xlib:drawable-height *window*))
  (xlib:display-finish-output *display*)
  (dolist (obj *obj-list*)
    (ecase (first obj)
      (:line (xlib:draw-line *window* *gc* (second obj) (third obj)
			     (fourth obj) (fifth obj)))
      (:rect (xlib:draw-rectangle *window* *gc* (second obj) (third obj)
				  (fourth obj) (fifth obj)))
      (:text (xlib:draw-glyphs *window* *gc* (second obj) (third obj)
			       (fourth obj)))
      (:circle (xlib:copy-area *circle* *gc* 0 0 *diameter* *diameter*
			       *window* (second obj) (third obj)))))
  (xlib:display-force-output *display*))

(defun display-scrawl-result (&aux (*print-pretty* #+cmu t #-cmu nil)
				   (*print-length* nil)
				   (*print-level* nil)
				   (*print-circle* t))
  (let* ((extract-string (get-car/cdr-string))
	 (text (if (> (length extract-string) *sdraw-display-width*)
		   (concatenate 'string
		    (subseq extract-string 0
			    (- *sdraw-display-width* 4))
		    "...)")
		   extract-string)))
    (sdraw *scrawl-current-obj*)
    (xlib:draw-glyphs *window* *gc* *window-h-offset*
		      (- (xlib:drawable-height *window*) *window-v-offset* 3)
		      text)
    (xlib:display-force-output *display*)))
