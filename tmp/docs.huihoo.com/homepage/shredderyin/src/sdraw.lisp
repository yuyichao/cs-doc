;;; -*- Mode: Lisp; Package: SDRAW -*-
;;;
;;; SDRAW - draws cons cell structures.
;;;
;;; From the book "Common Lisp:  A Gentle Introduction to
;;;      Symbolic Computation" by David S. Touretzky.  
;;; The Benjamin/Cummings Publishing Co., 1990.
;;;
;;; This is the generic version; it will work in any legal Common Lisp.
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

;; (in-package :cl-user)

;; (defpackage "SDRAW"
;;   (:use "CL" )
;;   (:nicknames "sdraw" "sd")
;;   (:export "SDRAW"))

;; (pushnew :sdraw *features*)


;; (in-package "SDRAW")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; The parameters below are in units of characters (horizontal)
;;; and lines (vertical).  They apply to all versions of SDRAW,
;;; but their values may change if cons cells are being drawn as
;;; bit maps rather than as character sequences.

(defparameter *sdraw-display-width* 79.)
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
    (let ((form (read)))
      (setf +++ ++
            ++  +
            +   -
            -   form)
      (if (eq form :abort) (return-from sdl1))
      (let ((result (eval form)))
        (setf /// //
              //  /
              /   (list result)
              *** **
              **  *
              *   result)
        (display-sdl-result *)))))

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
(defvar *scrawl-current-obj*)
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; The following definitions are specific to the tty implementation.

(defparameter *cons-string* "[*|*]")
(defparameter *cons-cell-flatsize* 5.)
(defparameter *cons-h-arrowshaft-char* #\-)
(defparameter *cons-h-arrowhead-char* #\>)
(defparameter *cons-v-line* "|")
(defparameter *cons-v-arrowhead* "v")

(defvar *textline-array* (make-array *sdraw-num-lines*))
(defvar *textline-lengths* (make-array *sdraw-num-lines*))

(eval-when (eval load)
  (dotimes (i *sdraw-num-lines*)
    (setf (aref *textline-array* i)
	  (make-array *sdraw-display-width*
		      :element-type 'string-char))))

(defun char-blt (row start-col string)
  (let ((spos (aref *textline-lengths* row))
	(line (aref *textline-array* row)))
    (do ((i spos (1+ i)))
	((>= i start-col))
      (setf (aref line i) #\Space))
    (replace line string :start1 start-col)
    (setf (aref *textline-lengths* row)
	  (+ start-col (length string)))))

(defun draw-structure (directions)
  (fill *textline-lengths* 0.)
  (when *sdraw-leading-arrow* (draw-leading-arrow))
  (follow-directions directions)
  (dump-display))

(defun draw-leading-arrow ()
  (do ((i 0 (1+ i)))
      ((>= (1+ i) *leading-arrow-length*)
       (char-blt 0 i (string *cons-h-arrowhead-char*)))
    (char-blt 0 i (string *cons-h-arrowshaft-char*))))

(defun follow-directions (dirs &optional is-car)
  (ecase (car dirs)
    (cons (draw-cons dirs))
    ((atom msg) (draw-msg dirs is-car))))

(defun draw-cons (obj)
  (let* ((row (second obj))
	 (col (third obj))
	 (car-component (fourth obj))
	 (cdr-component (fifth obj))
	 (string (sixth obj))
	 (line (aref *textline-array* row))
	 (h-arrow-start (+ col *cons-cell-flatsize*))
	 (h-arrowhead-col (1- (third cdr-component)))
	 (cdr-string? (if (eq 'cons (first cdr-component))
			  (sixth cdr-component)
			  (fifth cdr-component))))
    (if cdr-string? (decf h-arrowhead-col (length cdr-string?)))
    (char-blt row (- col (length string))
	      (if string (concatenate 'string string *cons-string*)
		  *cons-string*))
    (do ((i h-arrow-start (1+ i)))
	((>= i h-arrowhead-col))
      (setf (aref line i) *cons-h-arrowshaft-char*))
    (setf (aref line h-arrowhead-col) *cons-h-arrowhead-char*)
    (setf (aref *textline-lengths* row) (1+ h-arrowhead-col))
    (char-blt (+ row 1) (+ col 1) *cons-v-line*)
    (char-blt (+ row 2) (+ col 1) *cons-v-arrowhead*)
    (follow-directions car-component t)
    (follow-directions cdr-component)))

(defun draw-msg (obj is-car)
  (let* ((row (second obj))
	 (col (third obj))
	 (string (fourth obj))
	 (circ-string (fifth obj)))
    (if circ-string (setf string (concatenate 'string circ-string string)))
    (char-blt row
	      (+ (- col (length circ-string))
		 (if (and is-car
			  (<= (length string)
			      *cons-v-arrow-offset-threshold*))
		     *cons-v-arrow-offset-value*
		     0))
	      string)))

(defun dump-display ()
  (terpri)
  (dotimes (i *sdraw-num-lines*)
    (let ((len (aref *textline-lengths* i)))
      (if (plusp len)
	  (format t "~&~A"
		  (subseq (aref *textline-array* i) 0 len))
	  (return nil))))
  (terpri))
