;;; hide-region.el -- hide regions of text using overlays

;; Copyright (C) 2001  Mathias Dahl

;; Version: 1.0.0
;; Keywords: hide, region
;; Author: Mathias Dahl <MaTHiAS@SPAM_IS_BAD.VERY_BAD.dahl.NET>
;; Maintainer: Mathias Dahl
;; URL: http://mathias.dahl.net/pgm/emacs/elisp/hide-region.el

;; This file is not part of GNU Emacs.

;; This is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.
;;
;; This is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;; The function `hide-region-hide' hides the region. You can hide many
;; different regions and they will be "marked" by two configurable
;; strings (so that you know where the hidden text is).

;; The hidden regions is pushed on a kind of hide-region \"ring".

;; The function `hide-region-unhide' "unhides" one region, starting
;; with the last one you hid.

;; The best is to try it out. Test on the following:

;; Test region 1
;; Test region 2
;; Test region 3

;; If you are annoyed by the text getting "stuck" inside the hidden
;; regions, call the function `hide-region-setup-keybindings' to setup
;; local keybindings to a couple of functions trying to be smart and
;; guessing if the point is inside a hidden region and if so, move out
;; of it in the correct direction.

;;; Bugs

;; Probably many, but none that I know of. Comments and suggestions
;; are welcome!

(defgroup hide-region nil
  "Functions to hide region using an invisible overlay. The text is
not affected."
  :prefix "hide-region-"
  :group 'convenience)

(defcustom hide-region-before-string "@["
  "String to mark the beginning of an invisible region. This string is
not really placed in the text, it is just shown in the overlay"
  :type '(string)
  :group 'hide-region)

(defcustom hide-region-after-string "]@"
  "String to mark the beginning of an invisible region. This string is
not really placed in the text, it is just shown in the overlay"
  :type '(string)
  :group 'hide-region)

(defvar hide-region-overlays nil)

(defun hide-region-hide ()
  "Hides a region by making an invisible overlay over it and save the
overlay on the hide-region-overlays \"ring\""
  (interactive)
  (if (local-variable-p 'hide-region-overlays) 
      t (make-local-variable 'hide-region-overlays))
  (let ((new-overlay (make-overlay (mark) (point))))
    (setq hide-region-overlays 
	  (append 
	   (list new-overlay) hide-region-overlays))
    (overlay-put new-overlay 'invisible t)
    (overlay-put new-overlay 'before-string hide-region-before-string)
    (overlay-put new-overlay 'after-string hide-region-after-string)))

(defun hide-region-unhide ()
  "Unhide a region at a time, starting with the last one hidden and
deleting the overlay from the hide-region-overlays \"ring\"."
  (interactive)
  (if (car hide-region-overlays)
      (progn
	(delete-overlay (car hide-region-overlays))
	(setq hide-region-overlays (cdr hide-region-overlays)))
    (message "no hidden region left")))

(defun hide-region-inside-hidden ()
  "Are we inside a hidden region? If we are, return the overlay,
otherwise return nil."
  (let ((overlays hide-region-overlays)
	(overlay nil)
	(found nil))
    (while (and
	    (not found)
	    overlays)
      (progn
	(setq overlay (car overlays))
	(setq found
	      (and (<= (point) (overlay-end overlay))
		   (>= (point) (overlay-start overlay))))
	(setq overlays (cdr overlays))))
    (if found
	overlay
      nil)))

(defun hide-region-move-forward-over-overlay ()
  "Move \"forward\" until we get out of the overlay"
  (let ((overlay (hide-region-inside-hidden)))
    (if overlay
	(progn
	  (goto-char (+ (overlay-end overlay) 1))
	  t))))

(defun hide-region-move-backward-over-overlay ()
  "Move \"backward\" until we get out of the overlay"
  (let ((overlay (hide-region-inside-hidden)))
    (if overlay
	(progn
	  (goto-char (- (overlay-start overlay) 1))
	  t))))

(defun hide-region-next-line ()
  "Replacement for `next-line', trying to skip over hidden regions"
  (interactive)
  (if (not
       (and
	hide-region-overlays
	(hide-region-move-forward-over-overlay)))
      (next-line 1)))

(defun hide-region-forward-char ()
  "Replacement for `forward-char', trying to skip over hidden regions"
  (interactive)
  (if (not
       (and
	hide-region-overlays
	(hide-region-move-forward-over-overlay)))
      (forward-char 1)))

(defun hide-region-previous-line ()
  "Replacement for `previous-line', trying to skip over hidden regions"
  (interactive)
  (if (not
       (and
	hide-region-overlays
	(hide-region-move-backward-over-overlay)))
      (previous-line 1)))

(defun hide-region-backward-char ()
  "Replacement for `backward-char', trying to skip over hidden regions"
  (interactive)
  (if (not
       (and
	hide-region-overlays
	(hide-region-move-backward-over-overlay)))
      (backward-char 1)))

(defun hide-region-setup-keybindings ()
  "Setup local example keybindings for `hide-region'"
  (interactive)
  (global-set-key [down] 'hide-region-next-line)
  (global-set-key [up] 'hide-region-previous-line)
  (global-set-key [right] 'hide-region-forward-char)
  (global-set-key [left] 'hide-region-backward-char))

(defun hide-region-remove-keybindings ()
  "Remove local example keybindings for `hide-region'"
  (interactive)
  (local-unset-key [down])
  (local-unset-key [up])
  (local-unset-key [right])
  (local-unset-key [left]))

(hide-region-setup-keybindings)

(provide 'hide-region)