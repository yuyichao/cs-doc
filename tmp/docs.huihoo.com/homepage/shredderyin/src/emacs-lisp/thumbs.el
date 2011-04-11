;;; thumbs.el --- Thumbnails previewer for images files

;; Author: Jean-Philippe Theberge <yes0d@yahoo.fr>
;; Maintainer: Alex Schroeder <alex@gnu.org>
;; Keywords: Multimedia
;; Created: 15 Jan 2002
;; URL: http://www.emacswiki.org/cgi-bin/wiki.pl?ThumbsMode
;; Compatibility: Emacs 21 and Unix
;; Incompatibility: Emacs < 21, Xemacs, non-unix OS

;; $Id: thumbs.el,v 1.5 2003/04/16 21:43:27 alex Exp $

;; This is free software.  The GPL applies.

;;; Commentary:

;; This package modify dired mode and create two new mode: thumbs-mode
;; and thumbs-view-image-mode. It is used for images browsing and
;; viewing from within emacs. Minimal Image manipulation functions are
;; also available via external programs.
;;
;; The 'convert' program from 'ImageMagick'
;; [URL:http://www.imagemagick.org/] is required.
;;
;; The 'Esetroot' program from Eterm [URL:http://www.eterm.org/] is
;; optional (need by the desktop wallpaper functionality)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; NOTE: I will not make a XV front-end (to replace ImageMagick)
;;       because XV is not free software.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(require 'derived)
(require 'dired)
(require 'cl)

;; Abort if in-line imaging isn't supported (i.e. Emacs-20.7)
;;; Code:
(if (not (featurep 'image))
    (error "Emacs version %S doesn't support in-line images.
Upgrade to Emacs 21.1 or newer"
  emacs-version))

;;; User-configurable Code:
(defvar thumbs-thumbsdir
  (expand-file-name "~/.emacs-thumbs")
  "*Directory to store thumbnails.  Make sure it is expanded.")

(defvar thumbs-geometry "100x100"
  "*Size of thumbnails")

(defvar thumbs-per-line 5
  "*Number of thumbnails per line to show in directory.")

(defvar thumbs-thumbsdir-max-size 50000000
  "Max size for thumbnails directory.
When it reach that size (in bytes), a warning is send.
\(to be replaced with an auto delete of older files.)")

(defvar thumbs-conversion-program
  (or (locate-library "convert" t exec-path)
      "/usr/X11R6/bin/convert")
  "*Name of conversion program for thumbnails generation.
It must be 'convert'.")

(defvar thumbs-setroot-program
  (or (locate-library "Esetroot" t exec-path)
      "/usr/X11R6/bin/Esetroot")
  "*Name of Esetroot program for setting desktop Wallpaper.
Only 'Esetroot' is supported now but hack for another
program are more than welcome!")

(defvar thumbs-relief 5
  "*Size of button-like border around thumbnails.")

(defvar thumbs-margin 2
  "*Size of the margin around thumbnails.
This is where you see the cursor.")

(defvar thumbs-thumbsdir-auto-clean t
  "if true, auto-delete older file when the thumbnails directory
became bigger than 'thumbs-thumbsdir-max-size'.
If nil, just echo a warning.")

(defvar thumbs-image-resizing-step 10)

;; FIXME: security risk?
(defvar thumbs-temp-dir "/tmp/") ;; with ending slash

(defvar thumbs-temp-prefix "emacsthumbs") ;; without leading slash

(defvar thumbs-temp-file (concat thumbs-temp-dir thumbs-temp-prefix))

(defvar thumbs-current-tmp-filename nil
  "The filename of the image, if it has been modified.")

(defvar thumbs-html-width 6
  "* number of column in html generation page")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; end of user configurable code, don't edit below this line unless    ;;
;; you know what you are doing and, in this case, don't forget to send ;;
;; me your patches!                                                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Make sure auto-image-file-mode is ON.
(auto-image-file-mode t)

(defun thumbs-thumbsdir-size ()
  "Return the actual size (in bytes) of the thumbsnail dir."
  (apply '+ (mapcar
      (lambda (x)
        (nth 8 x))
      (directory-files-and-attributes
       thumbs-thumbsdir t
       (image-file-name-regexp)))))

(defun thumbs-cleanup-thumbsdir (dir max reg)
  "Clean DIR.
Deleting oldest files matching REG until DIR is below MAX bytes."
  (let* ((L (thumbs-sort-entries
      (directory-files-and-attributes
       dir t
       reg)))
  (dirsize
   (apply '+ (mapcar
       (lambda (x)
         (nth 8 x))
       L))))
    (while (> dirsize max)
      (progn
	(message (concat "Deleting file " (caar L)))
	(delete-file (caar L))
	(setq dirsize (- dirsize (nth 8 (car L))))
	(setq L (cdr L))))))

;; Check the thumbsnail directory size and clean it if necessary.
(if (> (thumbs-thumbsdir-size) thumbs-thumbsdir-max-size)
    (if thumbs-thumbsdir-auto-clean
 (thumbs-cleanup-thumbsdir thumbs-thumbsdir
      thumbs-thumbsdir-max-size
      (image-file-name-regexp))
      (message "Your thumbnails directory is huge!!")))

(defun thumbs-create-thumbsdir ()
  "If's thumbsdir don't exist, create it."
  (if (not (file-directory-p thumbs-thumbsdir))
      (progn
 (make-directory thumbs-thumbsdir)
 (message "Creating thumbnails directory"))))

(defun thumbs-call-convert (filein fileout action 
       &optional arg output-format action-prefix)
  (let ((command (format "%s %s%s %s \"%s\" \"%s:%s\""
    thumbs-conversion-program
    (or action-prefix "-")
    action
    (or arg "")
    filein
    (or output-format "jpeg")
    fileout)))
     (shell-command command)))

(defun thumbs-increment-image-size-element (n d)
  "Increment number N by D percent."
  (round (+ n (/ (* d n) 100))))

(defun thumbs-decrement-image-size-element (n d)
  "Decrement number N by D percent."
  (round (- n (/ (* d n) 100))))

(defun thumbs-increment-image-size (s)
  "Increment S (a cons of width x heigh)."
  (cons
   (thumbs-increment-image-size-element (car s) 
     thumbs-image-resizing-step)
   (thumbs-increment-image-size-element (cdr s) 
     thumbs-image-resizing-step)))
 
(defun thumbs-decrement-image-size (s)
  "Decrement S (a cons of width x heigh)."
  (cons
   (thumbs-decrement-image-size-element (car s) 
     thumbs-image-resizing-step)
   (thumbs-decrement-image-size-element (cdr s) 
     thumbs-image-resizing-step)))

(defun thumbs-resize-image (&optional increment size)
  "Resize image in current buffer.
if INCREMENT is set, make the image bigger, else smaller.
Or, alternatively, a SIZE may be specified."
  (interactive)
  ;; cleaning of old temp file
  (ignore-errors 
    (apply 'delete-file 
    (directory-files 
     thumbs-temp-dir t 
     thumbs-temp-prefix)))
  (let ((buffer-read-only nil)
	(x (if size
	       size
	     (if increment
		 (thumbs-increment-image-size
		  thumbs-current-image-size)
	       (thumbs-decrement-image-size
		thumbs-current-image-size))))
	(tmp (format "%s%s.jpg" thumbs-temp-file (gensym))))
    (erase-buffer)
    (thumbs-call-convert thumbs-current-image-filename
			 tmp "sample"
			 (concat (number-to-string (car x)) "x"
				 (number-to-string (cdr x))))
    (thumbs-insert-image tmp 'jpeg 0)
    (setq thumbs-current-tmp-filename tmp)))

(defun thumbs-resize-interactive (w h)
  "Resize Image interactively."
  (interactive "nWidth: \nnHeight: ")
  (thumbs-resize-image nil (cons w h)))
  
(defun thumbs-resize-image-size-down ()
  "Resize image."
  (interactive)
  (thumbs-resize-image nil))

(defun thumbs-resize-image-size-up ()
  "Resize image."
  (interactive)
  (thumbs-resize-image t))

(defsubst thumbs-compare-entries (l r inx func)
  "Compare the time of two files, L and R, the attribute indexed by INX."
  (let ((lt (nth inx (cdr l)))
 (rt (nth inx (cdr r))))
 (if (equal lt rt)
 (string-lessp (directory-file-name (car l))
        (directory-file-name (car r)))
      (funcall func rt lt))))

(defun thumbs-sort-entries (entries)
  "Sort ENTRIES, a list of files and attributes, by atime."
  (reverse (sort entries
   (function
    (lambda (l r)
      (let ((result
      (thumbs-compare-entries
       l r 4 'eshell-time-less-p)))
        result))))))

(defun thumbs-subst-char-in-string (orig rep string)
  "Replace occurrences of character ORIG with character REP in STRING.
Return the resulting (new) string. -- (defun borowed to Dave Love)"
  (let ((string (copy-sequence string))
 (l (length string))
 (i 0))
    (while (< i l)
      (if (= (aref string i) orig)
   (aset string i rep))
      (setq i (1+ i)))
    string))

(defun thumbs-thumbname (img)
  "Return a thumbnail name for the image IMG."
  (thumbs-subst-char-in-string
   ?\  ?\_
   (concat thumbs-thumbsdir "/"
    (apply 
     'concat
     (split-string
      (expand-file-name img) "/")))))

(defun thumbs-make-thumb (img)
  "Create the thumbnail for IMG."
  (thumbs-create-thumbsdir)
  (let* ((fn (expand-file-name img))
	 (tn (thumbs-thumbname img)))
    (if (not (file-exists-p tn))
	(thumbs-call-convert fn tn "sample" thumbs-geometry))
    tn))
  
(defun thumbs-image-type (img)
  "Return image type from filename IMG."
  (cond ((string-match ".*\.jpg$" img) 'jpeg)
	((string-match ".*\.gif$" img) 'gif)))

(defun thumbs-find-thumb (img)
  "Display the thumbnail for IMG."
  (interactive)
  (find-file (thumbs-make-thumb img)))

(defun thumbs-insert-image (img type relief)
  "Insert IMG at point.
Argument TYPE describe type."
  (let ((i `(image :type ,type
		   :file ,img
		   :relief ,relief
		   :margin ,thumbs-margin)))
    (insert-image i)
    (setq thumbs-current-image-size 
	  (image-size i t))))

(defun thumbs-insert-thumb (img)
  "Insert the thumbnail for IMG at point."
  (thumbs-insert-image
   (thumbs-make-thumb img)
   'jpeg thumbs-relief))

(defun thumbs-show-thumbs-list (L &optional buffer-name)
  "Make a preview buffer for all images in L."
  (pop-to-buffer (or buffer-name "*THUMB-View*"))
  (let ((buffer-read-only nil) (i 0))
    (erase-buffer)
    (thumbs-mode)
    (setq thumbs-fileL nil)
    (while L
      (when (= 0 (mod (setq i (1+ i)) thumbs-per-line))
	(newline))
      (setq thumbs-fileL (cons (cons (point)
				     (car L))
			       thumbs-fileL))
      (thumbs-insert-thumb (car L))
      (setq L (cdr L)))
    (goto-char (point-min))
    (make-variable-buffer-local
     'thumbs-fileL)))

(defun thumbs-show-all-from-dir (dir &optional reg)
  "Make a preview buffer for all images in DIR.
Optional argument REG to select file matching a regexp"
  (interactive "DDir: ")
  (thumbs-show-thumbs-list
   (directory-files dir t
      (or reg (image-file-name-regexp)))
   (concat "*Thumbs: " dir))
  (setq thumb-current-dir dir)
  (make-variable-buffer-local 'thumb-current-dir))

(defun thumbs-find-image-at-point (&optional img)
  "Display image for thumbnail at point in
the preview buffer."
  (interactive)
  (let* ((L thumbs-fileL)
	 (n (point))
	 (i (or img (cdr (assoc n L)))))
    (switch-to-buffer
     (concat "*Image: " (file-name-nondirectory i) " - " 
	     (number-to-string n) "*"))
    (thumbs-view-image-mode)
    (setq buffer-read-only nil)
    (make-variable-buffer-local 'thumbs-current-image-filename)
    (make-variable-buffer-local 'thumbs-current-tmp-filename)
    (make-variable-buffer-local 'thumbs-current-image-size)
    (make-variable-buffer-local 'thumbs-fileL)
    (make-variable-buffer-local 'thumbs-image-num)
    (delete-region (point-min)(point-max))
    (thumbs-insert-image i (thumbs-image-type i) 0)
    (setq thumbs-current-image-filename i
	  thumbs-fileL L
	  thumbs-current-tmp-filename nil
	  thumbs-image-num n
	  buffer-read-only t)))

(defun thumbs-find-image-at-point-other-window ()
  "Display image for thumbnail at point
in the preview buffer. Open another window."
  (interactive)
  (split-window)
  (next-window)
  (thumbs-find-image-at-point
   (cdr (assoc (point) thumbs-fileL))))

(defun thumbs-call-Esetroot (img)
  (shell-command 
   (concat thumbs-setroot-program " -fit " img)))

(defun thumbs-set-image-at-point-to-root-window ()
  "Use Esetroot to use the image at
point as a desktop wallpaper."
  (interactive)
  (thumbs-call-Esetroot (cdr (assoc (point) thumbs-fileL))))

;; FIXME: what is the difference to the previous one?
(defun thunbs-set-root ()
  "Set the current image as root."
  (interactive)
  (thumbs-call-Esetroot
   (or thumbs-current-tmp-filename
       thumbs-current-image-filename)))

(defun thumbs-delete-image-at-point ()
  "Delete the image at point (and it's thumbnail)."
  (interactive)
  (let ((f (cdr (assoc (point) thumbs-fileL))))
    (if (yes-or-no-p (concat "Really delete " f " "))
 (progn
   (delete-file f)
   (delete-file (thumbs-thumbname f)))))
  (thumbs-show-all-from-dir thumb-current-dir))

(defun thumbs-make-html ()
  (interactive)
  (let ((L thumbs-fileL)
 (count 0))
    (pop-to-buffer "*html*")
    (delete-region (point-min)(point-max))
    (insert "<html>\n<body>\n<table>\n")
    (while L
      (setq count (+ 1 count)) 
      (if (equal 1 count)(insert "<tr>\n"))
      (insert (concat "<td><a href=\"file:///" 
        (cdar L) 
        "\"><img src=\"file:///" 
        (thumbs-thumbname (cdar L)) 
        "\"></a></td>\n"))
      (setq L (cdr L))
      (if (equal thumbs-html-width count)
   (progn 
     (insert "</tr>\n")
     (setq count 0))))
    (insert "</tr>\n</table>\n</body>\n</html>\n")))

(defun thumbs-kill-buffer ()
  "Kill the current buffer"
  (interactive)
  (let ((buffer (current-buffer)))
    (ignore-errors (delete-window (selected-window)))
    ;; I must find another way to do this.
    (kill-buffer buffer)))

(defun thumbs-show-image-num (num)
  (interactive "nNumber: ")
  (setq buffer-read-only nil)
  (delete-region (point-min)(point-max))
  (let ((i (cdr (assoc num thumbs-fileL))))
    (thumbs-insert-image i (thumbs-image-type i) 0)
    (sleep-for 2)
    (rename-buffer (concat "*Image: " 
      (file-name-nondirectory i) 
      " - " 
      (number-to-string num) "*")))
  (setq thumbs-image-num num)
  (setq buffer-read-only t))

(defun thumbs-next-image ()
  (interactive)
  (thumbs-show-image-num (+ thumbs-image-num 1)))

(defun thumbs-previous-image ()
  (interactive)
  (thumbs-show-image-num (- thumbs-image-num 1)))

;; Image modification routines

(defun thumbs-modify-image (action &optional arg)
  "Call convert to modify the image."
  (interactive "sAction: \nsValue: ")
  ;; cleaning of old temp file
  (mapc 'delete-file
	(directory-files
	 thumbs-temp-dir
	 t
	 thumbs-temp-prefix))
  (let ((buffer-read-only nil)
	(tmp (format "%s%s.jpg" thumbs-temp-file (gensym))))
    (erase-buffer)
    (thumbs-call-convert thumbs-current-image-filename
			 tmp
			 action
			 (or arg ""))
    (thumbs-insert-image tmp 'jpeg 0)
    (setq thumbs-current-tmp-filename tmp))
  (setq buffer-read-only t))

(defun thumbs-emboss-image (emboss)
  "Emboss the image."
  (interactive "nEmboss value: ")
  (if (or (< emboss 3)(> emboss 31)(evenp emboss))
      (error "Arg must be a odd number between 3 and 31"))
  (thumbs-modify-image "emboss" (number-to-string emboss)))

(defun thumbs-monochrome-image ()
  "Turn the image to monochrome."
  (interactive)
  (thumbs-modify-image "monochrome"))

(defun thumbs-negate-image ()
  "Negate the image."
  (interactive)
  (thumbs-modify-image "negate"))

(defun thumbs-rotate-left ()
  "Rotate the image 90 degrees counter-clockwise."
  (interactive)
  (thumbs-modify-image "rotate" "270"))

(defun thumbs-rotate-right ()
  "Rotate the image 90 degrees clockwise."
  (interactive)
  (thumbs-modify-image "rotate" "90"))

(defun thumbs-forward-char ()
  "Move foreward one image."
  (interactive)
  (forward-char)
  (thumbs-show-name))

(defun thumbs-backward-char ()
  "Move backward one image."
  (interactive)
  (forward-char -1)
  (thumbs-show-name))

(defun thumbs-forward-line ()
  "Move down one line."
  (interactive)
  (next-line 1)
  (thumbs-show-name))

(defun thumbs-backward-line ()
  "Move up one line."
  (interactive)
  (next-line -1)
  (thumbs-show-name))

(defun thumbs-show-name ()
  "Show the name of the current file."
  (interactive)
  (message (cdr (assoc (point) thumbs-fileL))))

(defun thumbs-save-current-image ()
  "Save the current image."
  (interactive)
  (let ((f (or thumbs-current-tmp-filename 
	       thumbs-current-image-filename))
	(sa (read-from-minibuffer "save file as: " 
				  thumbs-current-image-filename)))
    (copy-file f sa)))

(defun thumbs-dired ()
  "Use `dired' on the current thumbs directory."
  (interactive)
  (dired thumb-current-dir))

(defun thumbs-show-all ()
  "View all images in the current thumbs directory."
  (interactive)
  (thumbs-show-all-from-dir thumb-current-dir))


;; thumbs-mode

(defvar thumbs-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map [return] 'thumbs-find-image-at-point)
    (define-key map [M-return] 'thumbs-find-image-at-point-other-window)
    (define-key map [C-return] 'thumbs-set-image-at-point-to-root-window)
    (define-key map [delete] 'thumbs-delete-image-at-point)
    (define-key map [right] 'thumbs-forward-char)
    (define-key map [left] 'thumbs-backward-char)
    (define-key map [up] 'thumbs-backward-line)
    (define-key map [down] 'thumbs-forward-line)
    (define-key map "d" 'thumbs-dired)
    (define-key map "g" 'thumbs-show-all)
    (define-key map "s" 'thumbs-show-name)
    (define-key map "q" 'thumbs-kill-buffer)
    map)
  "Keymap for `thumbs-mode'.")

(define-derived-mode thumbs-mode
  fundamental-mode "thumbs")

(defvar thumbs-view-image-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map [prior] 'thumbs-previous-image)
    (define-key map [next] 'thumbs-next-image)
    (define-key map "-" 'thumbs-resize-image-size-down)
    (define-key map "+" 'thumbs-resize-image-size-up)
    (define-key map "<" 'thumbs-rotate-left)
    (define-key map ">" 'thumbs-rotate-right)
    (define-key map "e" 'thumbs-emboss-image)
    (define-key map "r" 'thumbs-resize-interactive)
    (define-key map "s" 'thumbs-save-current-image)
    (define-key map "q" 'thumbs-kill-buffer)
    (define-key map "w" 'thunbs-set-root)
    map)
  "Keymap for `thumbs-view-image-mode'.")

;; thumbs-view-image-mode
(define-derived-mode thumbs-view-image-mode
  fundamental-mode "image-view-mode")

;; Modifications to dired-mode.

(defvar thumbs-show-preview nil)
(defvar thumbs-preview-buffer-name "*Preview*")
(defvar thumbs-preview-buffer-size 20)

(defun toggle-thumbs-show-preview ()
  (interactive)
  (if thumbs-show-preview
      (let ((w (get-buffer-window thumbs-preview-buffer-name)))
 (if w (progn (kill-buffer thumbs-preview-buffer-name)
       (delete-window w)))))
  (setq thumbs-show-preview (not thumbs-show-preview))
  (thumbs-dired-show-preview))

(defun thumbs-dired-show-preview ()
  (if thumbs-show-preview
      (let ((thumb-buffer-name
      thumbs-preview-buffer-name)
     (f (dired-get-filename))
     (old-buf (current-buffer)))
 (if (string-match (image-file-name-regexp) f)
     (progn
       (if (get-buffer-window thumb-buffer-name)
    (pop-to-buffer thumb-buffer-name)
  (progn
    (split-window
     (get-buffer-window (current-buffer))
     (- (window-width)
        thumbs-preview-buffer-size) t)
    (select-window (next-window))
    (switch-to-buffer thumb-buffer-name)))
       (progn
  (delete-region (point-min)(point-max))
  (thumbs-insert-thumb f)
  (pop-to-buffer old-buf t t)))))))

(defun thumbs-kill-preview-buffer-and-window ()
  (interactive)
  (let ((ob (current-buffer)))
    (pop-to-buffer thumbs-preview-buffer-name)
    (kill-buffer thumbs-preview-buffer-name)
    (delete-window)
    (pop-to-buffer ob)))

;; (defadvice dired-next-line (after show-thumbnail (arg))
;;   (thumbs-dired-show-preview))

;; (defadvice dired-previous-line (after show-thumbnail (arg))
;;   (thumbs-dired-show-preview))

;; (defadvice dired-advertised-find-file (before winkill)
;;   (thumbs-kill-preview-buffer-and-window))

;; (ad-activate 'dired-next-line)
;; (ad-activate 'dired-previous-line)
;; (ad-activate 'dired-advertised-find-file)

;; (define-key dired-mode-map "I" 'toggle-thumbs-show-preview)

;; (define-key dired-mode-map "T"
;;   #'(lambda ()
;;       (interactive)
;;       (thumbs-find-thumb (dired-get-filename))))
;; (define-key dired-mode-map "\C-t"
;;   #'(lambda()
;;       (interactive)
;;       (thumbs-show-all-from-dir (dired-current-directory))))
;; (define-key dired-mode-map "\M-t"
;;   #'(lambda()
;;       (interactive)
;;       (thumbs-show-thumbs-list
;;        (dired-get-marked-files)
;;        (concat "Thumbs : MARKED from " (dired-current-directory)))))
;; (define-key dired-mode-map "q"
;;   #'(lambda ()
;;       (interactive)
;;       (let ((w (get-buffer-window thumbs-preview-buffer-name)))
;;  (if w
;;      (progn
;;        (kill-buffer thumbs-preview-buffer-name)
;;        (delete-window w)))
;;  (quit-window))))
;; (define-key dired-mode-map "w"
;;   #'(lambda ()
;;       (interactive)
;;       (thumbs-call-Esetroot (dired-get-filename))))

(provide 'thumbs)

;;; thumbs.el ends here
