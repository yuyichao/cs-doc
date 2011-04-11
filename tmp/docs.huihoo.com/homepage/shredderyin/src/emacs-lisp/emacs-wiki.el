;;; emacs-wiki.el --- Maintain a local Wiki using Emacs-friendly markup

;; Copyright (C) 2001, 2002, 2003 John Wiegley (johnw AT gnu DOT org)

;; Emacs Lisp Archive Entry
;; Filename: emacs-wiki.el
;; Version: 2.39
;; Date: Sun 24-Nov-2002
;; Keywords: hypermedia
;; Author: John Wiegley (johnw AT gnu DOT org)
;;         Alex Schroeder (alex AT gnu DOT org)
;; Maintainer: Damien Elmes (emacswiki AT repose DOT cx)
;; Description: Maintain Emacs-friendly Wikis in a local directory
;; URL: http://repose.cx/emacs/wiki
;; Compatibility: Emacs20, Emacs21, XEmacs21

;; This file is not part of GNU Emacs.

;; The canonical URL for this file is now:
;;   http://repose.cx/emacs/wiki
;; Older copies and other modules which use emacs-wiki can be found at the
;; original author's page:
;;   http://www.gci-net.com/users/j/johnw/EmacsResources.html

;; This is free software; you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation; either version 2, or (at your option) any later
;; version.
;;
;; This is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
;; for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
;; MA 02111-1307, USA.

;;; Commentary:

;; Wiki is a concept, more than a thing.  It is a way of creating
;; document pages using plain text markup and simplified hyperlinking.

;; By typing a name in MixedCase, a hyperlink is automatically created
;; to the document "MixedCase".  Pressing return on that name will
;; create the file if it doesn't exist, or visit it if it does.

;; The markup used by emacs-wiki is intended to be very friendly to
;; people familiar with Emacs.  Type C-h v emacs-wiki-publishing-markup
;; after this mode is loaded for how to get started.

;; * Startup

;; To begin using emacs-wiki, put this in your .emacs file:

;;   (load "emacs-wiki")

;; Now you can type M-x emacs-wiki-find-file, give it a WikiName (or
;; just hit return) and start typing!

;; You should also type M-x customize-group, and give the name
;; "emacs-wiki".  Change it to suite your preferences.  Each of the
;; options has its own documentation.

;; * Keystroke summary

;; Here is a summary of keystrokes available in every Wiki buffer:

;;   C-c C-a    jump to an index of all the Wiki pages
;;   C-c C-b    show all pages that reference this page
;;   C-c C-s    search for a word in your Wiki pages
;;   C-c C-f    jump to another Wiki page; prompts for the name
;;   C-c C-l    highlight/refresh the current buffer
;;   C-c C-p    publish any Wiki pages that have changed as HTML
;;   C-c C-r    rename wiki link at point
;;   C-c C-v    change wiki project
;;   C-c C-D    delete wiki link at point (binding will only work on X)
;;   C-c =      diff this page against the last backup version
;;   TAB        move to the next Wiki reference
;;   S-TAB      move to the previous Wiki reference

;; * Using pcomplete

;; If you have pcomplete loaded, you can type M-TAB to complete Wiki
;; names.  Hitting M-TAB twice or more time in succession, will cycle
;; through all of the possibilities.  You can download pcomplete from
;; my Website:

;;   http://www.gci-net.com/~johnw/emacs.html

;; * ChangeLog support

;; If you use a ChangeLog (C-x 4 a) within one of your Wiki
;; directories, it will be used for notifying visitors to your wiki of
;; recent changes.

;; * Changing title or stylesheet

;; For convenience, if you want to change the visible title, or the
;; stylesheet, used by a certain Wiki page during HTML publishing,
;; just put:

;; #title Hello there
;; #style hello.css

;; at the top of the page.

;; * <lisp> tricks

;; <lisp></lisp> tags can be used, not only to evaluate forms for
;; insertion at that point, but to influence the publishing process in
;; many ways.  Here's another way to change a page's stylesheet:

;; <lisp>
;; (ignore
;;   ;; use special.css for this Wiki page
;;   (set (make-variable-buffer-local 'emacs-wiki-style-sheet)
;;        "<link rel=\"stylesheet\" type=\"text/css\" href=\"special.css\">"))
;; </lisp>

;; The 'ignore' is needed so nothing is inserted where the <lisp> tag
;; occurred.  Also, there should be no blank lines before or after the
;; tag (to avoid empty paragraphs from being created).  The best place
;; to put this would be at the very top or bottom of the page.

;; * Sub-lists?

;; There is no inherent support for sub-lists, since I couldn't think
;; of a simple way to do it.  But if you really need them, here's a
;; trick you can use:

;; - Hello
;;   <ul>
;;   <li>There
;;   <li>My friend
;;   </ul>

;;; Thanks

;; Alex Schroeder (alex AT gnu DOT org), current author of "wiki.el".
;;   His latest version is here:
;;       http://www.geocities.com/kensanata/wiki/WikiMode.html
;;
;; Frank Gerhardt (Frank.Gerhardt AT web DOT de), author of the original wiki-mode
;;   His latest version is here:
;;       http://www.s.netic.de/fg/wiki-mode/wiki.el
;;
;; Thomas Link (<t.link AT gmx DOT at)

;;; Code:

;; The parts of this code, and work to be done:
;;
;; * setup emacs-wiki major mode
;; * generate WikiName list
;; * utility functions to extract link parts
;; * open a page
;; * navigate links in the buffer
;; * visit a link
;; * search Wiki pages for text/backlinks
;; * index generation
;; * buffer highlighting (using font-lock)
;; * HTML publishing
;;   - Allow for alternate markup tables: DocBook, xhtml, etc.
;;   - <nop> used in a line of verse doesn't have effect
;; * HTTP serving (using httpd.el)
;;   - Diffing (look at using highlight-changes-mode and htmlify.el)
;;   - Editing (requires implementing POST method for httpd.el)

(defvar emacs-wiki-version "$Id"
  "The version of emacs-wiki currently loaded")

(require 'derived)

;; for caddr etc
;(eval-when-compile (require 'cl))

;; load pcomplete if it's available
(load "pcomplete" t t)

(defvar emacs-wiki-under-windows-p (memq system-type '(ms-dos windows-nt)))

;;; Options:

(defgroup emacs-wiki nil
  "Options controlling the behaviour of Emacs Wiki Mode.
Wiki is a concept, more than a thing.  It is a way of creating
document pages using plain text markup and simplified hyperlinking.

By typing a name in MixedCase, a hyperlink is automatically created
to the document \"MixedCase\".  Pressing return on that name will
create the file if it doesn't exist, or visit it if it does.

The markup used by emacs-wiki is intended to be very friendly to
people familiar with Emacs.  See the documentation for the variable
`emacs-wiki-publishing-markup' for a full description."
  :group 'hypermedia)

(defcustom emacs-wiki-mode-hook
  (append (if (featurep 'table)
	      '(table-recognize))
	  (unless (featurep 'httpd)
	    '(emacs-wiki-use-font-lock)))
  "A hook that is run when emacs-wiki mode is entered."
  :type 'hook
  :options '(emacs-wiki-use-font-lock
	     emacs-wiki-highlight-buffer
	     flyspell-mode
	     footnote-mode
	     highlight-changes-mode)
  :group 'emacs-wiki)

;;;###autoload
(defcustom emacs-wiki-directories '("~/Wiki")
  "A list of directories where Wiki pages can be found."
  :require 'emacs-wiki
  :type '(repeat :tag "Wiki directories" directory)
  :group 'emacs-wiki)

(defcustom emacs-wiki-default-page "WelcomePage"
  "Name of the default page used by \\[emacs-wiki-find-file]."
  :type 'string
  :group 'emacs-wiki)

(defcustom emacs-wiki-file-ignore-regexp
  "\\`\\(\\.?#.*\\|.*,v\\|.*~\\|\\.\\.?\\)\\'"
  "A regexp matching files to be ignored in Wiki directories."
  :type 'regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-ignored-extensions-regexp
  "\\.\\(bz2\\|gz\\|[Zz]\\)\\'"
  "A regexp of extensions to omit from the ending of Wiki page name."
  :type 'string
  :group 'emacs-wiki)

(defcustom emacs-wiki-interwiki-names
  '(("GnuEmacs" . "http://www.gnu.org/software/emacs/emacs.html")
    ("TheEmacsWiki" .
     (lambda (tag)
       (concat "http://www.emacswiki.org/cgi-bin/wiki.pl?"
               (or tag "EmacsWiki"))))
    ("MeatballWiki" .
     (lambda (tag)
       (concat "http://www.usemod.com/cgi-bin/mb.pl?"
	       (or tag "MeatballWiki")))))
  "A table of WikiNames that refer to external entities.
The format of this table is an alist, or series of cons cells.
Each cons cell must be of the form:

  (WIKINAME . STRING-OR-FUNCTION)

The second part of the cons cell may either be a STRING, which in most
cases should be a URL, or a FUNCTION.  If a function, it will be
called with one argument: the tag applied to the Interwiki name, or
nil if no tag was used.  If the cdr was a STRING and a tag is used,
the tag is simply appended.

Here are some examples:

  (\"JohnWiki\" . \"http://alice.dynodns.net/wiki?\")

Referring to [[JohnWiki#EmacsModules]] then really means:

  http://alice.dynodns.net/wiki?EmacsModules

If a function is used for the replacement text, you can get creative
depending on what the tag is.  Tags may contain any alphabetic
character, any number, % or _.  If you need other special characters,
use % to specify the hex code, as in %2E.  All browsers should support
this."
  :type '(repeat (cons (string :tag "WikiName")
		       (choice (string :tag "URL") function)))
  :group 'emacs-wiki)

(defvar emacs-wiki-url-or-name-regexp nil
  "Matches either a Wiki link or a URL.  This variable is auto-generated.")

(defvar emacs-wiki-url-or-name-regexp-group-count nil
  "Matches either a Wiki link or a URL.  This variable is auto-generated.")

(defcustom emacs-wiki-extended-link-regexp
  "\\[\\[\\([^] \t\n]+\\)\\]\\(\\[\\([^]\n]+\\)\\]\\)?\\]"
  "Regexp used to match [[extended][links]]."
  :type 'regexp
  :group 'emacs-wiki)

(defun emacs-wiki-count-chars (string char)
  (let ((i 0)
	(l (length string))
	(count 0))
    (while (< i l)
      (if (eq char (aref string i))
	  (setq count (1+ count)))
      (setq i (1+ i)))
    count))

(defun emacs-wiki-set-sym-and-url-regexp (sym value)
  (setq emacs-wiki-url-or-name-regexp
	(concat "\\("
		(if (eq sym 'emacs-wiki-name-regexp)
		    value
		  emacs-wiki-name-regexp) "\\|"
		(if (eq sym 'emacs-wiki-name-regexp)
		    (if (boundp 'emacs-wiki-url-regexp)
			emacs-wiki-url-regexp
		      "")
		  value) "\\)")
	emacs-wiki-url-or-name-regexp-group-count
	(- (emacs-wiki-count-chars
	    emacs-wiki-url-or-name-regexp ?\() 2))
  (set sym value))

(defcustom emacs-wiki-name-regexp
  (concat "\\(" emacs-wiki-extended-link-regexp "\\|"
	  "\\<[A-Z][a-z]+\\([A-Z][a-z]+\\)+\\(#[A-Za-z0-9_%]+\\)?" "\\)")
  "Regexp used to match WikiNames."
  :type 'regexp
  :set 'emacs-wiki-set-sym-and-url-regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-url-regexp
  (concat "\\<\\(https?:/?/?\\|ftp:/?/?\\|gopher://\\|"
	  "telnet://\\|wais://\\|file:/\\|s?news:\\|"
          "mailto:\\)"
	  "[^]	\n \"'()<>[^`{}]*[^]	\n \"'()<>[^`{}.,;]+")
  "A regexp used to match URLs within a Wiki buffer."
  :type 'regexp
  :set 'emacs-wiki-set-sym-and-url-regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-browse-url-function 'browse-url
  "Function to call to browse a URL."
  :type 'function
  :group 'emacs-wiki)

(defcustom emacs-wiki-grep-command
  "find %D -type f ! -name '*~' | xargs egrep -n -e \"\\<%W\\>\""
  "The name of the program to use when grepping for backlinks.
The string %D is replaced by `emacs-wiki-directories', space-separated.
The string %W is replaced with the name of the Wiki page.

Note: I highly recommend using glimpse to search large Wikis.  To use
glimpse, install and edit a file called .glimpse_exclude in your home
directory.  Put a list of glob patterns in that file to exclude Emacs
backup files, etc.  Then, run the indexer using:

  glimpseindex -o <list of Wiki directories>

Once that's completed, customize this variable to have the following
value:

  glimpse -nyi \"%W\"

Your searches will go much, much faster, especially for very large
Wikis.  Don't forget to add a user cronjob to update the index at
intervals."
  :type 'string
  :group 'emacs-wiki)

(defvar emacs-wiki-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map [(control ?c) (control ?a)] 'emacs-wiki-index)
    (define-key map [(control ?c) (control ?f)] 'emacs-wiki-find-file)
    (define-key map [(control ?c) (control ?b)] 'emacs-wiki-backlink)
    (define-key map [(control ?c) (control ?s)] 'emacs-wiki-search)
    (define-key map [(control ?c) (control ?p)] 'emacs-wiki-publish)
    (define-key map [(control ?c) (control ?v)] 'emacs-wiki-change-project)
    (define-key map [(control ?c) (control ?r)]
                                          'emacs-wiki-rename-link-at-point)
    (define-key map [(control ?c) (control ?D)]
                                          'emacs-wiki-delete-link-at-point)

    (define-key map [(control ?c) (control ?l)] 'font-lock-mode)

    (define-key map [(control ?c) ?=]
      (lambda ()
	(interactive)
	(diff-backup buffer-file-name)))

    (define-key map [tab] 'emacs-wiki-next-reference)
    (define-key map [(control ?i)] 'emacs-wiki-next-reference)

    (if (featurep 'xemacs)
	(define-key map [(shift tab)] 'emacs-wiki-previous-reference)
      (define-key map [(shift iso-lefttab)] 'emacs-wiki-previous-reference)
      (define-key map [(shift control ?i)] 'emacs-wiki-previous-reference))

    (when (featurep 'pcomplete)
      (define-key map [(meta tab)] 'pcomplete)
      (define-key map [(meta control ?i)] 'pcomplete))

    map)
  "Keymap used by Emacs Wiki mode.")

(defvar emacs-wiki-local-map
  (let ((map (make-sparse-keymap)))
    (define-key map [return] 'emacs-wiki-follow-name-at-point)
    (define-key map [(control ?m)] 'emacs-wiki-follow-name-at-point)
    (if (featurep 'xemacs)
	(define-key map [(button2)] 'emacs-wiki-follow-name-at-mouse)
      (define-key map [mouse-2] 'emacs-wiki-follow-name-at-mouse)
      (unless (eq emacs-major-version 21)
	(set-keymap-parent map emacs-wiki-mode-map)))
    map)
  "Local keymap used by emacs-wiki while on a WikiName.")

;; Code:

(defvar emacs-wiki-project nil)

;;;###autoload
(define-derived-mode emacs-wiki-mode text-mode "Wiki"
  "An Emacs mode for maintaining a local Wiki database.

Wiki is a hypertext and a content management system: Normal users are
encouraged to enhance the hypertext by editing and refactoring existing
wikis and by adding more.  This is made easy by requiring a certain way
of writing the wikis.  It is not as complicated as a markup language
such as HTML.  The general idea is to write plain ASCII.

Words with mixed case such as ThisOne are WikiNames.  WikiNames are
links you can follow.  If a wiki with that name exists, you will be
taken there.  If such a does not exist, following the link will create
a new wiki for you to fill.  WikiNames for non-existing wikis are
rendered as links with class \"nonexistent\", and are also displayed
in a warning color so that you can see wether following the link will
lead you anywhere or not.

In order to follow a link, hit RET when point is on the link, or use
mouse-2.

All wikis reside in the `emacs-wiki-directories'.

\\{emacs-wiki-mode-map}"
  (if emacs-wiki-project
      (emacs-wiki-change-project emacs-wiki-project))
  ;; because we're not inheriting from normal-mode, we need to
  ;; explicitly run file variables if the user wants to
  (condition-case err
      (hack-local-variables)
    (error (message "File local-variables error: %s"
		    (prin1-to-string err))))
  ;; bootstrap the file-alist, if it's not been read in yet
  (emacs-wiki-file-alist t)
  ;; if pcomplete is available, set it up!
  (when (featurep 'pcomplete)
    (set (make-variable-buffer-local 'pcomplete-default-completion-function)
	 'emacs-wiki-completions)
    (set (make-variable-buffer-local 'pcomplete-command-completion-function)
	 'emacs-wiki-completions)
    (set (make-variable-buffer-local 'pcomplete-parse-arguments-function)
	 'emacs-wiki-current-word)))

(defsubst emacs-wiki-page-file (page &optional no-check-p)
  "Return a filename if PAGE exists within the current Wiki."
  (cdr (assoc page (emacs-wiki-file-alist no-check-p))))

(defsubst emacs-wiki-directory-part (path)
  (directory-file-name (expand-file-name path)))

(defun emacs-wiki-directories-member (&optional directories)
  "Return non-nil if the current buffer is in `emacs-wiki-directories'."
  (let ((here (emacs-wiki-directory-part default-directory))
	(d (or directories emacs-wiki-directories))
	yes)
    (while d
      (if (string= here (emacs-wiki-directory-part (if (consp (car d))
						       (caar d)
						     (car d))))
	  (setq yes (car d) d nil)
	(setq d (cdr d))))
    yes))

(defun emacs-wiki-maybe (&optional check-only)
  "Maybe turn Emacs Wiki mode on for this file."
  (let ((projs emacs-wiki-projects)
	(mode-func 'emacs-wiki-mode)
	project yes)
    (while (and (not yes) projs)
      (let* ((projsyms (cdar projs))
	     (pred (assq 'emacs-wiki-predicate projsyms))
	     dirs)
	(if pred
	    (setq yes (funcall (cdr pred)))
	  (setq dirs (assq 'emacs-wiki-directories projsyms))
	  (if dirs
	      (setq yes (emacs-wiki-directories-member (cdr dirs)))))
	(if yes
	    (setq project (caar projs)
		  mode-func (or (cdr (assq 'emacs-wiki-major-mode projsyms))
				mode-func))))
      (setq projs (cdr projs)))
    (setq yes (or yes (emacs-wiki-directories-member)))
    (if (and yes (not check-only))
	(let ((emacs-wiki-project project))
	  (funcall mode-func)))
    yes))

(add-hook 'find-file-hooks 'emacs-wiki-maybe)

;;; Support WikiName completion using pcomplete

(defun emacs-wiki-completions ()
  "Return a list of possible completions names for this buffer."
  (while (pcomplete-here
	  (mapcar 'car (append (emacs-wiki-file-alist)
			       emacs-wiki-interwiki-names)))))

(defun emacs-wiki-current-word ()
  (let ((end (point)))
    (save-restriction
      (save-excursion
	(skip-chars-backward "^\\[ \t\n")
	(narrow-to-region (point) end))
      (pcomplete-parse-buffer-arguments))))

;;; Return an list of known wiki names and the files they represent.

(defsubst emacs-wiki-time-less-p (t1 t2)
  "Say whether time T1 is less than time T2."
  (or (< (car t1) (car t2))
      (and (= (car t1) (car t2))
	   (< (nth 1 t1) (nth 1 t2)))))

(defun emacs-wiki-page-name (&optional name)
  "Return the canonical form of the Wiki page name.
All this means is that certain extensions, like .gz, are removed."
  (save-match-data
    (unless name
      (setq name buffer-file-name))
    (if name
        (let ((page (file-name-nondirectory name)))
          (if (string-match emacs-wiki-ignored-extensions-regexp page)
              (replace-match "" t t page)
            page)))))

(defun emacs-wiki-page-title (&optional name)
  "Return the canonical form of the Wiki page name.
All this means is that certain extensions, like .gz, are removed."
  (or emacs-wiki-current-page-title
      (emacs-wiki-prettify-title (emacs-wiki-page-name name))))

(defvar emacs-wiki-file-alist nil)

(defun emacs-wiki-file-alist (&optional no-check-p)
  "Return possible Wiki filenames in `emacs-wiki-directories'.
On UNIX, this list is only updated if one of the directories' contents
have changed.  On Windows, it is always reread from disk."
  (let* ((file-alist (assoc emacs-wiki-current-project
			    emacs-wiki-file-alist))
	 (dirs emacs-wiki-directories)
	 (d dirs) last-mod)
    (unless (or emacs-wiki-under-windows-p no-check-p)
      (while d
	(let ((mod-time (nth 5 (file-attributes (car d)))))
	  (if (or (null last-mod)
		  (and mod-time (emacs-wiki-time-less-p last-mod mod-time)))
	      (setq last-mod mod-time)))
	(setq d (cdr d))))
    (if (or (and no-check-p (cadr file-alist))
	    (not (or emacs-wiki-under-windows-p
		     (null (cddr file-alist))
		     (null last-mod)
		     (emacs-wiki-time-less-p (cddr file-alist) last-mod))))
	(cadr file-alist)
      (if file-alist
	  (setcdr (cdr file-alist) last-mod)
	(setq file-alist (cons emacs-wiki-current-project (cons nil last-mod))
	      emacs-wiki-file-alist (cons file-alist emacs-wiki-file-alist)))
      (save-match-data
	(setcar
	 (cdr file-alist)
	 (let* ((names (list t))
		(lnames names))
	   (while dirs
	     (if (file-readable-p (car dirs))
		 (let ((files (directory-files (car dirs) t nil t)))
		   (while files
		     (unless
			 (or (file-directory-p (car files))
			     (string-match emacs-wiki-file-ignore-regexp
					   (file-name-nondirectory
					    (car files))))
		       (setcdr lnames
			       (cons (cons (emacs-wiki-page-name (car files))
					   (car files)) nil))
		       (setq lnames (cdr lnames)))
		     (setq files (cdr files)))))
	     (setq dirs (cdr dirs)))
	   (cdr names)))))))

(defun emacs-wiki-complete-alist ()
  "Return equivalent of calling (emacs-wiki-file-alist) for all projects."
  (let ((emacs-wiki-current-project "_CompositeFileList")
	(emacs-wiki-directories
	 (copy-alist emacs-wiki-directories))
	(projs emacs-wiki-projects))
    (while projs
      (let* ((projsyms (cdar projs))
	     (dirs (cdr (assq 'emacs-wiki-directories projsyms))))
	(while dirs
	  (add-to-list 'emacs-wiki-directories (car dirs))
	  (setq dirs (cdr dirs))))
      (setq projs (cdr projs)))
    (emacs-wiki-file-alist)))

;; Utility functions to extract parts of a Wiki name

(defvar emacs-wiki-serving-p nil
  "Non-nil when emacs-wiki is serving a wiki page directly.")

(defsubst emacs-wiki-transform-name (name)
  "Transform NAME as per `emacs-wiki-publishing-transforms', returning NAME"
  (save-match-data
    (mapc (function
           (lambda (elt)
             (let ((reg (car elt))
                   (rep (cdr elt)))
               (when (string-match reg name)
                 (setq name (replace-match rep t nil name))))))
          emacs-wiki-publishing-transforms)
    name))

(defsubst emacs-wiki-published-name (name &optional current)
  "Return the externally visible NAME for a wiki page, possibly transformed
  via `emacs-wiki-publishing-transforms'. If CURRENT is provided, convert any
  path to be relative to it"
  (emacs-wiki-transform-name
   (progn
     (when current
       (setq name (file-relative-name name
                                      (file-name-directory
                                       (emacs-wiki-transform-name current)))))
     (concat (if emacs-wiki-serving-p
                 (unless (string-match "\\?" name) "wiki?")
               emacs-wiki-publishing-file-prefix)
             name
             (if emacs-wiki-serving-p
                 (if emacs-wiki-current-project
                     (concat "&project=" emacs-wiki-current-project))
               emacs-wiki-publishing-file-suffix)))))

(defsubst emacs-wiki-published-file (&optional file)
  "Return the filename of the published file. Since this is based on the
  published-name, it will be filtered through
  `emacs-wiki-publishing-transforms'"
  (expand-file-name (emacs-wiki-published-name (emacs-wiki-page-name
                                                file))
                    emacs-wiki-publishing-directory))

(defcustom emacs-wiki-publishing-transforms nil
  "A list of cons cells mapping regexps to replacements, which is applied when
generating the published name from the wiki file name. The replacements
run in order so you can chain them together.

An example is how I publish the emacs-wiki documentation. The emacs-wiki
homepage is in a file called EmacsWiki. With the following settings I can
publish directly to my webserver via tramp (the first rule catches 'WikiMarkup'
for instance):

(setq emacs-wiki-publishing-directory \"/webserver:/var/www/\")
(setq emacs-wiki-publishing-transforms
        ((\".*Wiki.*\" . \"emacs/wiki/\\&\")
         (\"EmacsWiki\\|WelcomePage\" . \"index\")))

Then when trying to publish a page EmacsWiki:

(emacs-wiki-published-file \"EmacsWiki\")

You get:

\"/webserver:/var/www/emacs/wiki/index.html\""
  :type '(repeat
	  (cons
	   (regexp :tag "String to match")
	   (string :tag "Replacement string")))
  :group 'emacs-wiki-publish)

(defsubst emacs-wiki-wiki-url-p (name)
  "Return non-nil if NAME is a URL."
  (save-match-data
    (string-match emacs-wiki-url-regexp name)))

(defun emacs-wiki-wiki-visible-name (wiki-name)
  "Return the visible part of a Wiki link.
This only really means something if [[extended][links]] are involved."
  (save-match-data
    (let ((name wiki-name))
      (if (string-match emacs-wiki-extended-link-regexp name)
	  (if (match-string 2 name)
	      (setq name (match-string 3 name))
	    (setq name (match-string 1 name))))
      (if (and (not (emacs-wiki-wiki-url-p name))
	       (string-match "#" name))
	  (if (= 0 (match-beginning 0))
	      (setq name (emacs-wiki-page-name))
	    (let ((base (substring name 0 (match-beginning 0))))
	      (if (assoc base emacs-wiki-interwiki-names)
		  (setq name (concat (substring name 0 (match-beginning 0))
				     ":" (substring name (match-end 0))))
		(setq name base)))))
      name)))

(defun emacs-wiki-wiki-tag (wiki-name)
  (save-match-data
    (if (string-match "#" wiki-name)
	(substring wiki-name (match-end 0)))))

(defun emacs-wiki-wiki-link-target (wiki-name)
  "Return the target of a Wiki link.  This might include anchor tags."
  (save-match-data
    (let ((name wiki-name) lookup)
      (if (string-match "^\\[\\[\\([^]]+\\)\\]" name)
	  (setq name (match-string 1 name)))
      (if (and emacs-wiki-interwiki-names
	       (string-match "\\`\\([^#]+\\)\\(#\\(.+\\)\\)?\\'" name)
	       (setq lookup (assoc (match-string 1 name)
				   emacs-wiki-interwiki-names)))
	  (let ((tag (match-string 3 name))
		(target (cdr lookup)))
	    (if (stringp target)
		(setq name (concat target tag))
	      (setq name (funcall target tag))))
	(if (and (> (length name) 0)
		 (eq (aref name 0) ?#))
	    (setq name (concat (emacs-wiki-page-name) name))))
      name)))

(defun emacs-wiki-wiki-base (wiki-name)
  "Find the WikiName or URL mentioned by a Wiki link.
This means without tags, in the case of a WikiName."
  (save-match-data
    (let ((file (emacs-wiki-wiki-link-target wiki-name)))
      (if (emacs-wiki-wiki-url-p file)
	  file
	(if (string-match "#" file)
	    (substring file 0 (match-beginning 0))
	  file)))))

;;; Open a Wiki page (with completion)

(defvar emacs-wiki-history-list nil)

(defun emacs-wiki-read-name (file-alist)
  "Read the name of a valid Wiki page from minibuffer, with completion."
  (if (featurep 'xemacs)
      (let ((str (completing-read
		  (format "Wiki page: (default: %s) "
			  emacs-wiki-default-page)
		  (emacs-wiki-file-alist)
		  nil nil nil 'emacs-wiki-history-list)))
	(if (or (null str) (= (length str) 0))
	    emacs-wiki-default-page
	  str))
    (completing-read
     (format "Wiki page: (default: %s) " emacs-wiki-default-page)
     file-alist nil nil nil 'emacs-wiki-history-list
     emacs-wiki-default-page)))

;;;###autoload
(defun emacs-wiki-find-file (wiki &optional command directory)
  "Open the Emacs Wiki page WIKI by name.
If COMMAND is non-nil, it is the function used to visit the file.
If DIRECTORY is non-nil, it is the directory in which the Wiki page
will be created if it does not already exist."
  (interactive
   (list
    (let ((num (prefix-numeric-value current-prefix-arg)))
       (if (< num 16)
	   (let* ((file-alist (if (= num 4)
				  (emacs-wiki-complete-alist)
				(emacs-wiki-file-alist)))
		  (name (emacs-wiki-read-name file-alist)))
	     (cons name (cdr (assoc name file-alist))))
	 (let ((name (read-file-name "Open wiki file: ")))
	   (cons name name))))))
  (unless (interactive-p)
    (setq wiki (cons wiki
		     (cdr (assoc wiki (emacs-wiki-file-alist))))))
  ;; At this point, `wiki' is (GIVEN-PAGE FOUND-FILE).
  (if (cdr wiki)
      (let ((buffer (funcall (or command 'find-file) (cdr wiki))))
	(if (= (prefix-numeric-value current-prefix-arg) 16)
	    (with-current-buffer buffer
	      (set (make-variable-buffer-local 'emacs-wiki-directories)
		   (cons (file-name-directory (cdr wiki))
			 emacs-wiki-directories))
	      (set (make-variable-buffer-local 'emacs-wiki-file-alist) nil)))
	buffer)
    (let* ((dirname (or directory
			(emacs-wiki-maybe t)
			(car emacs-wiki-directories)))
	   (filename (expand-file-name (car wiki) dirname)))
      (unless (file-exists-p dirname)
	(make-directory dirname t))
      (funcall (or command 'find-file) filename))))

;;; Navigate/visit links or URLs.  Use TAB, S-TAB and RET (or mouse-2).

(defun emacs-wiki-next-reference ()
  "Move forward to next Wiki link or URL, cycling if necessary."
  (interactive)
  (let ((case-fold-search nil)
	(cycled 0) pos)
    (save-excursion
      (if (emacs-wiki-link-at-point)
	  (goto-char (match-end 0)))
      (while (< cycled 2)
	(if (re-search-forward emacs-wiki-url-or-name-regexp nil t)
	    (setq pos (match-beginning 0)
		  cycled 2)
	  (goto-char (point-min))
	  (setq cycled (1+ cycled)))))
    (if pos
	(goto-char pos))))

(defun emacs-wiki-previous-reference ()
  "Move backward to the next Wiki link or URL, cycling if necessary.
This function is not entirely accurate, but it's close enough."
  (interactive)
  (let ((case-fold-search nil)
	(cycled 0) pos)
    (save-excursion
      (while (< cycled 2)
	(if (re-search-backward emacs-wiki-url-or-name-regexp nil t)
	    (setq pos (point)
		  cycled 2)
	  (goto-char (point-max))
	  (setq cycled (1+ cycled)))))
    (if pos
	(goto-char pos))))

(defun emacs-wiki-visit-link (link-name)
  "Visit the URL or link named by LINK-NAME."
  (let ((link (emacs-wiki-wiki-link-target link-name)))
    (if (emacs-wiki-wiki-url-p link)
	(funcall emacs-wiki-browse-url-function link)
      ;; The name list is current since the last time the buffer was
      ;; highlighted
      (let* ((base (emacs-wiki-wiki-base link-name))
	     (file (emacs-wiki-page-file base t))
	     (tag  (and (not (emacs-wiki-wiki-url-p link))
			(emacs-wiki-wiki-tag link))))
	(if (null file)
	    (find-file base)
	  (find-file file)
	  (when tag
	    (goto-char (point-min))
	    (re-search-forward (concat "^\\.?#" tag) nil t)))))))

(unless (fboundp 'line-end-position)
  (defsubst line-end-position (&optional N)
    (save-excursion (end-of-line N) (point))))

(unless (fboundp 'line-beginning-position)
  (defsubst line-beginning-position (&optional N)
    (save-excursion (beginning-of-line N) (point))))

(unless (fboundp 'match-string-no-properties)
  (defalias 'match-string-no-properties 'match-string))

(defun emacs-wiki-link-at-point (&optional pos)
  "Return non-nil if a URL or Wiki link name is at point."
  (if (or (null pos)
	  (and (char-after pos)
	       (not (eq (char-syntax (char-after pos)) ? ))))
      (let ((case-fold-search nil)
	    (here (or pos (point))))
	(save-excursion
	  (goto-char here)
	  (skip-chars-backward "^'\"<>{}( \t\n")
	  (or (looking-at emacs-wiki-url-or-name-regexp)
	      (and (search-backward "[[" (line-beginning-position) t)
		   (looking-at emacs-wiki-name-regexp)
		   (<= here (match-end 0))))))))

(defun emacs-wiki-follow-name-at-point ()
  "Visit the link at point, or insert a newline if none."
  (interactive)
  (if (emacs-wiki-link-at-point)
      (emacs-wiki-visit-link (match-string 0))
    (error "There is no valid link at point")))

(defun emacs-wiki-follow-name-at-mouse (event)
  "Visit the link at point, or yank text if none."
  (interactive "e")
  (save-excursion
    (cond ((fboundp 'event-window)	; XEmacs
	   (set-buffer (window-buffer (event-window event)))
	   (and (event-point event) (goto-char (event-point event))))
	  ((fboundp 'posn-window)	; Emacs
	   (set-buffer (window-buffer (posn-window (event-start event))))
	   (goto-char (posn-point (event-start event)))))
    (if (emacs-wiki-link-at-point)
	(emacs-wiki-visit-link (match-string 0)))))

(defun emacs-wiki-rename-link (link-name new-name)
  (when (emacs-wiki-wiki-url-p link-name)
    (error "Can't rename a URL"))
  (let* ((base (emacs-wiki-wiki-base link-name))
         (file (emacs-wiki-page-file base t)))
    (if (null file)
        (rename-file base new-name)
      (rename-file file new-name))))

(defun emacs-wiki-rename-link-at-point ()
  "Rename the link under point, and the location it points to. This does not
  work with URLs"
  (interactive "*")
  (let (new-name old-name)
    (if (emacs-wiki-link-at-point)
        (progn
          (setq old-name (match-string 0))
          ;; emacs21 leaves the local keymap on this string, so we must strip
          ;; properties so the user can hit return to exit minibuf
          (set-text-properties 0 (length old-name) nil old-name)
          (setq new-name (read-from-minibuffer "Rename to: " old-name))
          (emacs-wiki-rename-link old-name new-name)
          ;; at this point, the file would have been successfully renamed, so
          ;; it's safe to change to link name now
          (replace-match new-name nil t))
      (error "There is no valid link at point"))))

(defun emacs-wiki-delete-link (link-name)
  "Delete the file which link-name corresponds to"
  (when (emacs-wiki-wiki-url-p link-name)
    (error "Can't rename a URL"))
  (let* ((base (emacs-wiki-wiki-base link-name))
         (file (emacs-wiki-page-file base t)))
    (if (null file)
        (delete-file base)
      (delete-file file))))

(defun emacs-wiki-delete-link-at-point ()
  "Delete the link under point, and the location it points to. This does not
  work with URLs"
  (interactive "*")
  (let (name)
    (if (emacs-wiki-link-at-point)
        (progn
          (setq name (match-string 0))
          (when (yes-or-no-p (concat "Delete "
                                     name "? You can not undo this. "))
            (emacs-wiki-delete-link name)
            (replace-match "" nil t)))
      (error "There is no valid link at point"))))

;;; Find text in Wiki pages, or pages referring to the current page

(defvar emacs-wiki-search-history nil)

(defun emacs-wiki-grep (string &optional grep-command)
  "Grep for STRING in the Wiki directories. GREP-COMMAND if passed will
  supplant emacs-wiki-grep-command."
  (require 'compile)
  (let ((str (or grep-command emacs-wiki-grep-command))
	(dirs (mapconcat (lambda (dir)
                           (shell-quote-argument (expand-file-name dir)))
			 emacs-wiki-directories " ")))
    (while (string-match "%W" str)
      (setq str (replace-match string t t str)))
    (while (string-match "%D" str)
      (setq str (replace-match dirs t t str)))
    (compile-internal str "No more search hits" "search"
		      nil grep-regexp-alist)))

(defun emacs-wiki-search (text)
  "Search for the given TEXT string in the Wiki directories."
  (interactive
   (list (let ((str (concat emacs-wiki-grep-command)) pos)
	   (when (string-match "%W" str)
             (setq pos (match-beginning 0))
             (unless (featurep 'xemacs)
               (setq pos (1+ pos)))
	     (setq str (replace-match "" t t str)))
	   (read-from-minibuffer "Search command: "
				 (cons str pos)
				 nil nil 'emacs-wiki-search-history))))
  (emacs-wiki-grep nil text))

(defun emacs-wiki-backlink ()
  "Grep for the current pagename in all the Wiki directories."
  (interactive)
  (emacs-wiki-grep (emacs-wiki-page-name)))

;;; Generate an index of all known Wiki pages

(defun emacs-wiki-generate-index (&optional as-list exclude-private)
  "Generate an index of all Wiki pages."
  (let ((project emacs-wiki-current-project))
    (with-current-buffer (get-buffer-create "*Wiki Index*")
      (erase-buffer)
      (if project
	  (emacs-wiki-change-project project))
      (let ((files (sort (copy-alist (emacs-wiki-file-alist))
			 (function
			  (lambda (l r)
			    (string-lessp (car l) (car r))))))
	    file)
	(while files
	  (unless (and exclude-private
		       (emacs-wiki-private-p (caar files)))
	    (insert (if as-list "- " "") "[[" (caar files) "]]\n"))
	  (setq files (cdr files))))
      (current-buffer))))

(defun emacs-wiki-index ()
  "Display an index of all known Wiki pages."
  (interactive)
  (message "Generating Wiki index...")
  (pop-to-buffer (emacs-wiki-generate-index))
  (goto-char (point-min))
  (emacs-wiki-mode)
  (message "Generating Wiki index...done"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Emacs Wiki Highlighting
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defgroup emacs-wiki-highlight nil
  "Options controlling the behaviour of Emacs Wiki highlighting.
See `emacs-wiki-highlight-buffer' for more information."
  :group 'emacs-wiki)

(defun emacs-wiki-make-faces ()
  (mapc (lambda (newsym)
          (let (num)
            (setq num newsym)
            (setq newsym (intern (concat "emacs-wiki-header-"
                                         (int-to-string num))))
	    (cond
	     ((featurep 'xemacs)
              (eval `(defface ,newsym
                       '((t (:size
                             ,(nth (1- num) '("24pt" "18pt" "14pt" "12pt"))
                             :bold t)))
                       "emacs-wiki header face"
                       :group 'emacs-wiki-highlight)))
	     ((< emacs-major-version 21)
	      (copy-face 'default newsym))
	     (t
	      (eval `(defface ,newsym
		       '((t (:height ,(1+ (* 0.1 (- 5 num)))
				     :inherit variable-pitch
				     :weight bold)))
		       "emacs-wiki header face"
		       :group 'emacs-wiki-highlight))))))
	'(1 2 3 4)))
(emacs-wiki-make-faces)

(defface emacs-wiki-link-face
  '((((class color) (background light))
     (:foreground "green" :underline "green" :bold t))
    (((class color) (background dark))
     (:foreground "cyan" :underline "cyan" :bold t))
    (t (:bold t)))
  "Face for Wiki cross-references."
  :group 'emacs-wiki-highlight)

(defface emacs-wiki-bad-link-face
  '((((class color) (background light))
     (:foreground "red" :underline "red" :bold t))
    (((class color) (background dark))
     (:foreground "coral" :underline "coral" :bold t))
    (t (:bold t)))
  "Face for bad Wiki cross-references."
  :group 'emacs-wiki-highlight)

(defcustom emacs-wiki-highlight-buffer-hook nil
  "A hook run after a region is highlighted.
Each function receives three arguments: BEG END VERBOSE.
BEG and END mark the range being highlighted, and VERBOSE specifies
whether progress messages should be displayed to the user."
  :type 'hook
  :group 'emacs-wiki-highlight)

(defcustom emacs-wiki-inline-images (and (not (featurep 'xemacs))
					 (>= emacs-major-version 21)
					 window-system)
  "If non-nil, inline locally available images within Wiki pages."
  :type 'boolean
  :group 'emacs-wiki-highlight)

(defcustom emacs-wiki-image-regexp
  "\\.\\(eps\\|gif\\|jp\\(e?g\\)\\|p\\(bm\\|ng\\)\\|tiff\\|x\\([bp]m\\)\\)\\'"
  "A link matching this regexp will be published inline as an image. Remember
that it must be matched as a link first - so use either [[CamelCaps]] or
include a leading slash - [[./text]]. An example:

  [[./wife.jpg][A picture of my wife]]

If you omit the description, the alt tag of the resulting HTML buffer will be
the name of the file."
  :type 'regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-file-regexp
  "[/?]\\|\\.\\(html?\\|pdf\\|el\\|zip\\|txt\\|tar\\)\\(\\.\\(gz\\|bz2\\)\\)?\\'"
  "A link matching this regexp will be regarded as a link to a file. Remember
that it must be matched as a link first - so use either [[CamelCaps]] or
include a leading slash - [[./text]]"
  :type 'regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-tag-regexp
  "<\\([^/ \t\n][^ \t\n</>]*\\)\\(\\s-+[^<>]+[^</>]\\)?\\(/\\)?>"
  "A regexp used to find XML-style tags within a buffer when publishing.
Group 1 should be the tag name, group 2 the properties, and group
3 the optional immediate ending slash."
  :type 'regexp
  :group 'emacs-wiki)

(defcustom emacs-wiki-inline-relative-to 'emacs-wiki-publishing-directory
  "The name of a symbol which records the location relative to where images
  should be found. The default assumes that when editing, the images can be
  found in the publishing directory. Another sensible default is
  `default-directory', which will try and find the images relative to the
  local page. You can use this to store images in wikidir/images, and
  maintain a parallel copy on the remote host."
  :type 'symbol
  :group 'emacs-wiki)

(defcustom emacs-wiki-markup-tags
  '(("example" t nil t emacs-wiki-example-tag)
    ("verbatim" t nil t emacs-wiki-verbatim-tag)
    ("nowiki" t nil t emacs-wiki-nowiki-tag)
    ("verse" t nil nil emacs-wiki-verse-tag)
    ("numbered" t nil nil emacs-wiki-numbered-tag)
    ("nop" nil nil t emacs-wiki-nop-tag)
    ("contents" nil t nil emacs-wiki-contents-tag)
    ("c-source" t t t emacs-wiki-c-source-tag))
  "A list of tag specifications, for specially marking up Wiki text.
XML-style tags are the best way to add custom markup to Emacs Wiki.
This is easily accomplished by customizing this list of markup tags.

For each entry, the name of the tag is given, whether it expects a
closing tag and/or an optional set of attributes, if the handler
function can also highlight the tag, and a function that performs
whatever action is desired within the delimited region.

The tags themselves are deleted during publishing, although not during
highlighting, before the function is called.  The function is called
with three arguments, the beginning and end of the region surrounded
by the tags (including the tags themselves, in the case of
highlighting).  The third argument indicates whether the purpose of
the call is to highlight the region, or mark it up for publishing.  If
properties are allowed, they are passed as a fourth argument in the
form of an alist.  The `end' argument to the function is always a
marker.

Point is always at the beginning of the region within the tags, when
the function is called.  Wherever point is when the function finishes
is where tag markup/highlighting will resume.

These tag rules are processed once at the beginning of markup, and
once at the end, to catch any tags which may have been inserted
in-between.  For highlighting, they are processed as they occur, in
the order they occur, once per text region.

Here is a summary of the default tags.  This includes the dangerous
tags listed in `emacs-wiki-dangerous-tags', which may not be used by
outsiders.

 verbatim
   Protects against highlighting and wiki interpretation, and escapes any
   characters which have special meaning to the publishing format. For HTML,
   this means characters like '<' are escaped as HTML entities.

 example
   Like verbatim, but typesets in HTML using the <pre> tag, with
   class=example, so whitespace formatting is preserved.

 nowiki
   Inhibits wiki markup, but does not do any escaping to the underlying
   publishing medium. Useful for embedding HTML, PHP, etc.

 verse
   Typesets like a normal paragraph, but without word-wrapping.
   That is, whitespace is preserved.

 redirect
   Using the \"url\" attribute, you can specify that a page should
   redirect to another page.  The remaining contents of the page will
   not be published.  The optional \"delay\" attribute specifies how
   long to wait before redirecting.

 nop
   When placed before a WikiLink, it will prevent that WikiLink from
   being treated as such.  Good for names like DocBook.

 contents
   Produces a compact table of contents for any section heading at the
   same level or lower than the next section header encountered.
   Optional \"depth\" attribute specifies how deep the table of
   contents should go.

 lisp
   Evaluate the region as a Lisp form, and displays the result.  When
   highlighting, the `display' text property is used, preserving the
   underlying text.  Turn off font-lock mode if you wish to edit it.

 command
   Pass the region to a command interpretor and insert the result,
   guarding it from any further expansion.  Optional \"file\"
   attribute specifies the shell or interpretor to use.  If none is
   given, and `emacs-wiki-command-tag-file' has not been configured,
   Eshell is used.

 python, perl
   Pass the region to the Python or Perl language interpretor, and
   insert the result.

 c-source
   Markup the region as C or C++ source code, using the c2html
   program, if available.  Optional boolean attribute \"numbered\"
   will cause source lines to be numbered.

   Note: If c2html is not available, the region will be converted to
   HTML friendly text (i.e., <> turns into &lt;&gt;), and placed in a
   <pre> block.  In this case, line numbering is not available.

 bookmarks
   Insert bookmarks at the location of the tag from the given
   bookmarks file.  Required attribute \"file\" specifies which file
   to read from, and the optional attribute \"type\" may be one of:
   adr (for Opera), lynx, msie, ns, xbel or xmlproc.  The default type
   is \"xbel\".  The optional attribute \"folder\" may be used to
   specify which folder (and its children) should be inserted.

   Note that xml-parse.el version 1.5 (available from my website) and
   the xbel-utils package (available at least to Debian users) is
   required for this feature to work."
  :type '(repeat (list (string :tag "Markup tag")
		       (boolean :tag "Expect closing tag" :value t)
		       (boolean :tag "Parse attributes" :value nil)
		       (boolean :tag "Highlight tag" :value nil)
		       function))
  :group 'emacs-wiki-highlight)

(defcustom emacs-wiki-dangerous-tags
  '(("redirect" t t nil emacs-wiki-redirect-tag)
    ("lisp" t nil t emacs-wiki-lisp-tag)
    ("command" t t t emacs-wiki-command-tag)
    ("python" t t t emacs-wiki-python-tag)
    ("perl" t t t emacs-wiki-perl-tag)
    ("bookmarks" nil t nil emacs-wiki-bookmarks-tag))
  "A list of tag specifications, for specially marking up Wiki text.
These tags are dangerous -- meaning represent a gaping security hole
-- and therefore are not available to outsiders who happen to edit a
Wiki page"
  :type '(repeat (list (string :tag "Markup tag")
		       (boolean :tag "Expect closing tag" :value t)
		       (boolean :tag "Parse attributes" :value nil)
		       (boolean :tag "Highlight tag" :value nil)
		       function))
  :group 'emacs-wiki-highlight)

(defvar emacs-wiki-highlight-regexp nil)
(defvar emacs-wiki-highlight-vector nil)

(defun emacs-wiki-configure-highlighting (sym val)
  (setq emacs-wiki-highlight-regexp
	(concat "\\(" (mapconcat (function
				  (lambda (rule)
				    (if (symbolp (car rule))
					(symbol-value (car rule))
				      (car rule)))) val "\\|") "\\)")
	emacs-wiki-highlight-vector (make-vector 128 nil))
  (let ((rules val))
    (while rules
      (if (eq (cadr (car rules)) t)
	  (let ((i 0) (l 128))
	    (while (< i l)
	      (unless (aref emacs-wiki-highlight-vector i)
		(aset emacs-wiki-highlight-vector i
		      (nth 2 (car rules))))
	      (setq i (1+ i))))
	(aset emacs-wiki-highlight-vector (cadr (car rules))
	      (nth 2 (car rules))))
      (setq rules (cdr rules))))
  (set sym val))

(defsubst emacs-wiki-highlight-ok-context-p (beg end str)
  "Ensures whitespace or punctuation comes before the position BEG, and
  after the string STR. A search-forward is done for STR, bounding by END, and
  the position of the end of the match is returned if in the correct context."
  (save-excursion
    (let ((len (length str)))
      (and
       (setq end (search-forward str end t))
       ;; post end, want eob or whitespace/punctuation
       (or (> (skip-syntax-forward ". " (1+ end)) 0)
           (eq nil (char-after end)))
       (goto-char (- end len))
       ;; pre end, no whitespace
       (eq (skip-syntax-backward " " (- end len 1)) 0)
       (goto-char (+ beg len))
       ;; post beg, no whitespace
       (eq (skip-syntax-forward " " (+ beg len 1)) 0)
       (or (backward-char len) t) ;; doesn't return anything useful
       ;; pre beg, want sob or whitespace/punctuation
       (or (< (skip-syntax-backward ". " (1- beg)) 0)
           (eq nil (char-before beg)))
       end))))

(defun emacs-wiki-multiline-maybe (beg end &optional predicate)
  "If region between beg-end is a multi-line region, and the optional
  predicate is true, font lock the current region as multi-line. Predicate is
  called with the excursion saved."
  (when (and (or (eq (char-before end) ?\n)
                 (> (count-lines beg end) 1))
             (or (not predicate)
                 (save-excursion (funcall predicate beg end))))
    (save-excursion
      ;; mark whole lines as a multiline font-lock
      (goto-char beg)
      (setq beg (line-beginning-position))
      (goto-char end)
      (setq end (line-end-position))
      (add-text-properties beg end '(font-lock-multiline t))
      t)))

(defun emacs-wiki-highlight-emphasized ()
  ;; here we need to check four different points - the start and end of the
  ;; leading *s, and the start and end of the trailing *s. we allow the
  ;; outsides to be surrounded by whitespace or punctuation, but no word
  ;; characters, and the insides must not be surrounded by whitespace or
  ;; punctuation. thus the following are valid:
  ;; " *foo bar* "
  ;; "**foo**,"
  ;; and the following is invalid:
  ;; "** testing **"
  (let* ((beg (match-beginning 0))
	 (e1 (match-end 0))
	 (leader (- e1 beg))
         (end end)
	 b2 e2 face)
    ;; if it's a header
    (unless (save-excursion
              (goto-char beg)
              (when (save-match-data (looking-at "^\\*\\{1,3\\} "))
                (add-text-properties
                 (line-beginning-position) (line-end-position)
                 (list 'face
                       (intern (concat "emacs-wiki-header-"
                                       (int-to-string (1+ leader))))))
                t))
      ;; it might be an normal, emphasised piece of text
      (when (and
             (setq e2 (emacs-wiki-highlight-ok-context-p
                       beg end (buffer-substring-no-properties beg e1)))
             (setq b2 (match-beginning 0)))
        (cond ((= leader 1) (setq face 'italic))
              ((= leader 2) (setq face 'bold))
              ((= leader 3) (setq face 'bold-italic)))
        (add-text-properties beg e1 '(invisible t intangible t))
        (add-text-properties e1 b2 (list 'face face))
        (add-text-properties b2 e2 '(invisible t intangible t)))
      (emacs-wiki-multiline-maybe
       beg end
       ;; ensures we only mark the region as multiline if it's correctly
       ;; delimited at the start
       (lambda (beg end)
         (goto-char (1+ beg))
         (eq (skip-syntax-forward " " (1+ beg)) 0)
         (or (backward-char) t)
         (or (< (skip-syntax-backward ". " (1- beg)) 0)
             (eq nil (char-before beg))))))))

(defun emacs-wiki-highlight-underlined ()
  (let ((start (- (point) 2))
        end)
    (when (setq end (emacs-wiki-highlight-ok-context-p start end "_"))
      (add-text-properties start (+ start 1) '(invisible t intangible t))
      (add-text-properties (+ start 1) (- end 1) '(face underline))
      (add-text-properties (- end 1) end '(invisible t intangible t)))))

(defun emacs-wiki-highlight-verbatim ()
  (let ((start (- (point) 2))
        end)
    (when (setq end (emacs-wiki-highlight-ok-context-p start end "="))
        (search-forward "=" end t))))

(defcustom emacs-wiki-highlight-markup
  `(;; render in teletype and suppress further parsing
    ("=[^\t =]" ?= emacs-wiki-highlight-verbatim)

    ;; make emphasized text appear emphasized
    ("\\*+" ?* emacs-wiki-highlight-emphasized)

    ;; make underlined text appear underlined
    ("_[^ \t_]" ?_ emacs-wiki-highlight-underlined)

    ;; make quadruple quotes invisible
    ("''''" ?\'
     ,(function
       (lambda ()
	 (add-text-properties (match-beginning 0) (match-end 0)
			      '(invisible t intangible t)))))

    ("^#title" ?\# emacs-wiki-highlight-title)

    (emacs-wiki-url-or-name-regexp t emacs-wiki-highlight-link)

    ;; highlight any markup tags encountered
    (emacs-wiki-tag-regexp ?\< emacs-wiki-highlight-custom-tags))
  "Expressions to highlight an Emacs Wiki buffer.
These are arranged in a rather special fashion, so as to be as quick as
possible.

Each element of the list is itself a list, of the form:

  (LOCATE-REGEXP TEST-CHAR MATCH-FUNCTION)

LOCATE-REGEXP is a partial regexp, and should be the smallest possible
regexp to differentiate this rule from other rules.  It may also be a
symbol containing such a regexp.  The buffer region is scanned only
once, and LOCATE-REGEXP indicates where the scanner should stop to
look for highlighting possibilities.

TEST-CHAR is a char or t.  The character should match the beginning
text matched by LOCATE-REGEXP.  These chars are used to build a vector
for fast MATCH-FUNCTION calling.

MATCH-FUNCTION is the function called when a region has been
identified.  It is responsible for adding the appropriate text
properties to change the appearance of the buffer.

This markup is used to modify the appearance of the original text to
make it look more like the published HTML would look (like making some
markup text invisible, inlining images, etc).

font-lock is used to apply the markup rules, so that they can happen
on a deferred basis.  They are not always accurate, but you can use
\\[font-lock-fontifty-block] near the point of error to force
fontification in that area.

Lastly, none of the regexp should contain grouping elements that will
affect the match data results."
  :type '(repeat
	  (list :tag "Highlight rule"
		(choice (regexp :tag "Locate regexp")
			(symbol :tag "Regexp symbol"))
		(choice (character :tag "Confirm character")
			(const :tag "Default rule" t))
		function))
  :set 'emacs-wiki-configure-highlighting
  :group 'emacs-wiki-highlight)

(defvar font-lock-mode nil)
(defvar font-lock-multiline nil)

(defun emacs-wiki-use-font-lock ()
  (set (make-local-variable 'font-lock-multiline) 'undecided)
  (set (make-local-variable 'font-lock-defaults)
       `(nil t nil nil 'beginning-of-line
	 (font-lock-fontify-region-function . emacs-wiki-highlight-region)
	 (font-lock-unfontify-region-function
	  . emacs-wiki-unhighlight-region)))
  (set (make-local-variable 'font-lock-fontify-region-function)
       'emacs-wiki-highlight-region)
  (set (make-local-variable 'font-lock-unfontify-region-function)
       'emacs-wiki-unhighlight-region)
  (font-lock-mode t))

(defun emacs-wiki-mode-flyspell-verify ()
  "Return t if the word at point should be spell checked."
  (let* ((word-pos (1- (point)))
	 (props (text-properties-at word-pos)))
    (not (or (bobp)
	     (memq 'display props)
	     (if (and font-lock-mode (cadr (memq 'fontified props)))
		 (memq (cadr (memq 'face props))
		       '(emacs-wiki-link-face emacs-wiki-bad-link-face))
	       (emacs-wiki-link-at-point word-pos))))))

(put 'emacs-wiki-mode 'flyspell-mode-predicate
     'emacs-wiki-mode-flyspell-verify)

(defun emacs-wiki-eval-lisp (form)
  "Evaluate the given form and return the result as a string."
  (require 'pp)
  (save-match-data
    (let ((object (eval (read form))))
      (cond
       ((stringp object) object)
       ((and (listp object)
	     (not (eq object nil)))
	(let ((string (pp-to-string object)))
	  (substring string 0 (1- (length string)))))
       ((numberp object)
	(number-to-string object))
       ((eq object nil) "")
       (t
	(pp-to-string object))))))

(defun emacs-wiki-highlight-buffer ()
  "Re-highlight the entire Wiki buffer."
  (interactive)
  (emacs-wiki-highlight-region (point-min) (point-max) t))

(defun emacs-wiki-highlight-region (beg end &optional verbose)
  "Apply highlighting according to `emacs-wiki-highlight-markup'.
Note that this function should NOT change the buffer, nor should any
of the functions listed in `emacs-wiki-highlight-markup'."
  (let ((buffer-undo-list t)
	(inhibit-read-only t)
	(inhibit-point-motion-hooks t)
	(inhibit-modification-hooks t)
	(modified-p (buffer-modified-p))
	deactivate-mark)
    (unwind-protect
	(save-excursion
	  (save-restriction
	    (widen)
	    ;; check to see if we should expand the beg/end area for
	    ;; proper multiline matches
	    (when (and font-lock-multiline
		       (> beg (point-min))
		       (get-text-property (1- beg) 'font-lock-multiline))
	      ;; We are just after or in a multiline match.
	      (setq beg (or (previous-single-property-change
			     beg 'font-lock-multiline)
			    (point-min)))
	      (goto-char beg)
	      (setq beg (line-beginning-position)))
	    (when font-lock-multiline
	      (setq end (or (text-property-any end (point-max)
					       'font-lock-multiline nil)
			    (point-max))))
	    (goto-char end)
	    (setq end (line-beginning-position 2))
	    ;; Undo any fontification in the area.
	    (font-lock-unfontify-region beg end)
	    ;; And apply fontification based on `emacs-wiki-highlight-markup'
	    (let ((len (float (- end beg)))
		  (case-fold-search nil))
	      (goto-char beg)
	      (while
		  (and (< (point) end)
		       (re-search-forward emacs-wiki-highlight-regexp end t))
		(if verbose
		    (message "Highlighting buffer...%d%%"
			     (* (/ (float (- (point) beg)) len) 100)))
		(funcall (aref emacs-wiki-highlight-vector
			       (char-after (match-beginning 0)))))
	      (run-hook-with-args 'emacs-wiki-highlight-buffer-hook
				  beg end verbose)
	      (if verbose (message "Highlighting buffer...done")))))
      (set-buffer-modified-p modified-p))))

(defun emacs-wiki-unhighlight-region (begin end &optional verbose)
  "Remove all visual highlights in the buffer (except font-lock)."
  (let ((buffer-undo-list t)
	(inhibit-read-only t)
	(inhibit-point-motion-hooks t)
	(inhibit-modification-hooks t)
	(modified-p (buffer-modified-p))
	deactivate-mark)
    (unwind-protect
	(remove-text-properties
	 begin end '(face nil font-lock-multiline nil
			  invisible nil intangible nil display nil
			  mouse-face nil keymap nil help-echo nil))
      (set-buffer-modified-p modified-p))))

(eval-when-compile
  (defvar end))

(defun emacs-wiki-multiline-maybe (beg end &optional predicate)
  "If region between beg-end is a multi-line region, and the optional
  predicate is true, font lock the current region as multi-line. Predicate is
  called with the excursion saved."
  (when (and (or (eq (char-before end) ?\n)
                 (> (count-lines beg end) 1))
             (or (not predicate)
                 (save-excursion (funcall predicate beg end))))
    (save-excursion
      ;; mark whole lines as a multiline font-lock
      (goto-char beg)
      (setq beg (line-beginning-position))
      (goto-char end)
      (setq end (line-end-position))
      (add-text-properties beg end '(font-lock-multiline t))
      t)))

(defvar emacs-wiki-keymap-property
  (if (or (featurep 'xemacs)
	  (>= emacs-major-version 21))
      'keymap
    'local-map))

(defsubst emacs-wiki-link-properties (help-str &optional face)
  (append (if face
	      (list 'face face 'rear-nonsticky t
                    emacs-wiki-keymap-property emacs-wiki-local-map)
	    (list 'invisible t 'intangible t 'rear-nonsticky t
                  emacs-wiki-keymap-property emacs-wiki-local-map))
	  (list 'mouse-face 'highlight
		'help-echo help-str
		emacs-wiki-keymap-property emacs-wiki-local-map)))

(defun emacs-wiki-highlight-link ()
  (if (eq ?\[ (char-after (match-beginning 0)))
      (if (and emacs-wiki-inline-images
	       (save-match-data
		 (string-match emacs-wiki-image-regexp (match-string 4))))
	  (emacs-wiki-inline-image (match-beginning 0) (match-end 0)
				   (match-string 4) (match-string 6))
	(let* ((link (match-string-no-properties 4))
	       (invis-props (emacs-wiki-link-properties link))
	       (props (emacs-wiki-link-properties link 'emacs-wiki-link-face)))
	  (if (match-string 6)
	      (progn
		(add-text-properties (match-beginning 0)
				     (match-beginning 6) invis-props)
		(add-text-properties (match-beginning 6) (match-end 6) props)
		(add-text-properties (match-end 6) (match-end 0) invis-props))
	    (add-text-properties (match-beginning 0)
				 (match-beginning 4) invis-props)
	    (add-text-properties (match-beginning 4) (match-end 0) props)
	    (add-text-properties (match-end 4) (match-end 0) invis-props)))
	(goto-char (match-end 0)))
    (if (and emacs-wiki-inline-images
	     (save-match-data
	       (string-match emacs-wiki-image-regexp (match-string 0))))
	(emacs-wiki-inline-image (match-beginning 0) (match-end 0)
				 (match-string 0))
      (add-text-properties
       (match-beginning 0) (match-end 0)
       (emacs-wiki-link-properties
	(match-string-no-properties 0)
	(if (let ((base (emacs-wiki-wiki-base (match-string 0))))
	      (or (emacs-wiki-page-file base t)
		  (save-match-data
		    (string-match "\\(/\\|\\`[a-z]\\{3,6\\}:\\)" base))))
	    'emacs-wiki-link-face
	  'emacs-wiki-bad-link-face)))
      (goto-char (match-end 0)))))

(defun emacs-wiki-inline-image (beg end url &optional desc)
  "Inline locally available images."
  (let ((filename
	 (cond
	  ((string-match "\\`file:\\(.+\\)" url)
	   (match-string 1 url))
	  ((string-match "/" url)
	   (expand-file-name url (symbol-value
                                  emacs-wiki-inline-relative-to))))))
    (if (and filename (file-readable-p filename))
	(add-text-properties beg end (list 'display (create-image filename)
					   'help-echo (or desc url))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Emacs Wiki Publishing (to HTML by default)
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defgroup emacs-wiki-publish nil
  "Options controlling the behaviour of Emacs Wiki publishing.
See `emacs-wiki-publish' for more information."
  :group 'emacs-wiki)

(defcustom emacs-wiki-maintainer (concat "mailto:webmaster@" (system-name))
  "URL where the maintainer can be reached."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-home-page emacs-wiki-default-page
  "Title of the Wiki Home page."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-index-page "WikiIndex"
  "Title of the Wiki Index page."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-downcase-title-words
  '("the" "and" "at" "on" "of" "for" "in" "an" "a")
  "Strings that should be downcased in a Wiki page title."
  :type '(repeat string)
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-use-mode-flags (not emacs-wiki-under-windows-p)
  "If non-nil, use file mode flags to determine page permissions.
Otherwise the regexps in `emacs-wiki-private-pages' and
`emacs-wiki-editable-pages' are used."
  :type 'boolean
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-private-pages nil
  "A list of regexps to exclude from public view.
This variable only applies if `emacs-wiki-use-mode-flags' is nil."
  :type '(choice (const nil) (repeat regexp))
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-editable-pages nil
  "A list of regexps of pages that may be edited via HTTP.
This variable only applies if `emacs-wiki-use-mode-flags' is nil."
  :type '(choice (const nil) (repeat regexp))
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-directory "~/WebWiki"
  "Directory where all wikis are published to."
  :type 'directory
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-file-prefix ""
  "This prefix will be prepended to all wiki names when publishing."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-file-suffix ".html"
  "This suffix will be appended to all wiki names when publishing."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-before-markup-hook nil
  "A hook run in the buffer where markup is done, before it is done."
  :type 'hook
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-after-markup-hook nil
  "A hook run in the buffer where markup is done, after it is done."
  :type 'hook
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-meta-http-equiv "Content-Type"
  "The http-equiv attribute used for the HTML <meta> tag."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-meta-content-type "text/html"
  "The content type used for the HTML <meta> tag."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-meta-content-coding
  (if (featurep 'mule)
      'detect
    "iso-8859-1")
  "If set to the symbol 'detect, use `emacs-wiki-coding-map' to try
  and determine the HTML charset from emacs's coding. If set to a string, this
  string will be used to force a particular charset"
  :type '(choice string symbol)
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-charset-default "iso-8859-1"
  "The default HTML meta charset to use if no translation is found in
  `emacs-wiki-coding-map'"
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-coding-default 'iso-8859-1
  "The default emacs coding  use if no special characters are found"
  :type 'symbol
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-coding-map
  '((iso-2022-jp "iso-2022-jp")
    (utf-8 "utf-8"))
  "An alist mapping emacs coding systems to appropriate HTML charsets.
  Use the base name of the coding system (ie, without the -unix)"
  :type '(alist :key-type coding-system :value-type (group string))
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-redirect-delay 1
  "The number of seconds to delay before doing a page redirect."
  :type 'integer
  :group 'emacs-wiki-publish)

(defvar emacs-wiki-current-page-title nil
  "Current page title, used instead of buffer name if non-nil.
This is usually set by code called by `emacs-wiki-publishing-markup'.
It should never be changed globally.")

(defcustom emacs-wiki-anchor-on-word nil
  "When true, anchors surround the closest word. This allows you
to select them in a browser (ie, for pasting), but has the
side-effect of marking up headers in multiple colours if your
header style is different to your link style."
  :type 'boolean
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-header
  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">
<html>
  <head>
    <title><lisp>(emacs-wiki-page-title)</lisp></title>
    <meta name=\"generator\" content=\"emacs-wiki.el\">
    <meta http-equiv=\"<lisp>emacs-wiki-meta-http-equiv</lisp>\"
	  content=\"<lisp>emacs-wiki-meta-content</lisp>\">
    <link rev=\"made\" href=\"<lisp>emacs-wiki-maintainer</lisp>\">
    <link rel=\"home\" href=\"<lisp>(emacs-wiki-published-name
				     emacs-wiki-home-page)</lisp>\">
    <link rel=\"index\" href=\"<lisp>(emacs-wiki-published-name
				      emacs-wiki-index-page)</lisp>\">
    <lisp>emacs-wiki-style-sheet</lisp>
  </head>
  <body>
    <h1><lisp>(emacs-wiki-page-title)</lisp></h1>
    <!-- Page published by Emacs Wiki begins here -->\n"
  "Text to prepend to a wiki being published.
This text may contain <lisp> markup tags."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-footer
  "
    <!-- Page published by Emacs Wiki ends here -->
    <div class=\"navfoot\">
      <hr>
      <table width=\"100%\" border=\"0\" summary=\"Footer navigation\">
	<tr>
	  <td width=\"33%\" align=\"left\">
	    <lisp>
	      (if buffer-file-name
		  (concat
		   \"<span class=\\\"footdate\\\">Updated: \"
		   (format-time-string emacs-wiki-footer-date-format
		    (nth 5 (file-attributes buffer-file-name)))
		   (and emacs-wiki-serving-p
			(emacs-wiki-editable-p (emacs-wiki-page-name))
			(concat
			 \" / \"
			 (emacs-wiki-link-href
			  (concat \"editwiki?\" (emacs-wiki-page-name))
			  \"Edit\")))
		   \"</span>\"))
	    </lisp>
	  </td>
	  <td width=\"34%\" align=\"center\">
	    <span class=\"foothome\">
	      <lisp>
		(concat
		 (and (emacs-wiki-page-file emacs-wiki-home-page t)
		      (not (emacs-wiki-private-p emacs-wiki-home-page))
		      (concat
		       (emacs-wiki-link-href emacs-wiki-home-page \"Home\")
		       \" / \"))
		 (emacs-wiki-link-href emacs-wiki-index-page \"Index\")
		 (and (emacs-wiki-page-file \"ChangeLog\" t)
		      (not (emacs-wiki-private-p \"ChangeLog\"))
		      (concat
		       \" / \"
		       (emacs-wiki-link-href \"ChangeLog\" \"Changes\"))))
	      </lisp>
	    </span>
	  </td>
	  <td width=\"33%\" align=\"right\">
	    <lisp>
	      (if emacs-wiki-serving-p
		  (concat
		   \"<span class=\\\"footfeed\\\">\"
		   (emacs-wiki-link-href \"searchwiki?get\" \"Search\")
		   (and buffer-file-name
			(concat
			 \" / \"
			 (emacs-wiki-link-href
			  (concat \"searchwiki?q=\" (emacs-wiki-page-name))
			  \"Referrers\")))
		   \"</span>\"))
	    </lisp>
	  </td>
	</tr>
      </table>
    </div>
  </body>
</html>\n"
  "Text to append to a wiki being published.
This text may contain <lisp> markup tags."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-footer-date-format "%Y-%m-%d"
  "Format of current date for `emacs-wiki-publishing-footer'.
This string must be a valid argument to `format-time-string'."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-style-sheet
  "<style type=\"text/css\">
a.nonexistent {
  font-weight: bold;
  background-color: #F8F8F8; color: #FF2222;
}

a.nonexistent:visited {
  background-color: #F8F8F8; color: #FF2222;
}

body {
  background: white; color: black;
  margin-left: 5%; margin-right: 5%;
  margin-top: 3%;
}

em { font-style: italic; }
strong { font-weight: bold; }

ul { list-style-type: disc }

dl.contents { margin-top: 0; }
dt.contents { margin-bottom: 0; }

p.verse {
  white-space: pre;
  margin-left: 5%;
}

pre {
  white-space: pre;
  font-family: monospace;
  margin-left: 5%;
}
</style>"
  "The style sheet used for each wiki page.
This can either be an inline stylesheet, using <style></style> tags,
or an external stylesheet reference using a <link> tag.

Here is an example of using a <link> tag:

  <link rel=\"stylesheet\" type=\"text/css\" href=\"emacs-wiki.css\">"
  :type 'string
  :group 'emacs-wiki-publish)

(defvar emacs-wiki-publishing-p nil
  "Set to t while Wiki pages are being published.
This can be used by <lisp> tags to know when HTML is being generated.")

(defcustom emacs-wiki-block-groups-regexp
  "\\(h[1-9r]\\|[oud]l\\|table\\|center\\|blockquote\\|pre\\)[^>]*"
  "This regexp identifies HTML tag which defines their own blocks.
That is, they do not need to be surrounded by <p>."
  :type 'regexp
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-table-attributes "border=\"2\" cellpadding=\"5\""
  "The attribute to be used with HTML <table> tags.
Note that since emacs-wiki support direct insertion of HTML tags, you
can easily create any kind of table you want, as long as every line
begins at column 0 (to prevent it from being blockquote'd).  To make
really ANYTHING you want, use this idiom:

  <verbatim>
  <table>
    [... contents of my table, in raw HTML ...]
  </verbatim></table>

It may look strange to have the tags out of sequence, but remember
that verbatim is processed long before table is even seen."
  :type 'string
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-report-threshhold 100000
  "If a Wiki file is this size or larger, report publishing progress."
  :type 'integer
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-publishing-markup
  (list
   ["&\\([-A-Za-z_#0-9]+\\);" 0 emacs-wiki-markup-entity]

   ;; change the displayed title or the stylesheet for a given page
   ["\\`#\\(title\\|style\\)\\s-+\\(.+\\)\n+" 0
    emacs-wiki-markup-initial-directives]

   ;; process any markup tags
   [emacs-wiki-tag-regexp 0 emacs-wiki-markup-custom-tags]

   ;; emphasized or literal text
   ["\\(^\\|[-[ \t\n<('`\"]\\)\\(=[^= \t\n]\\|_[^_ \t\n]\\|\\*+[^* \t\n]\\)"
    2 emacs-wiki-markup-word]

   ;; headings, outline-mode style
   ["^\\(\\*+\\)\\s-+" 0 emacs-wiki-markup-heading]

   ;; define anchor points
   ["^#\\([A-Za-z0-9_%]+\\)\\s-*" 0 emacs-wiki-markup-anchor]

   ;; horizontal rule, or section separator
   ["^----+" 0 "<hr>"]

   ;; footnotes section is separated by a horizontal rule in HTML
   ["^\\(\\* \\)?Footnotes:?\\s-*" 0 "<hr>\n<p>\n"]
   ;; footnote definition/reference (def if at beginning of line)
   ["\\[\\([1-9][0-9]*\\)\\]" 0 emacs-wiki-markup-footnote]

   ;; don't require newlines between numbered and unnumbered lists.
   ;; This must come before paragraphs are calculated, so that any
   ;; extra newlines added will be condensed.
   ["^\\s-*\\(-\\|[0-9]+\\.\\)" 1 "\n\\1"]

   ;; the beginning of the buffer begins the first paragraph
   ["\\`\n*" 0 "<p>\n"]
   ;; plain paragraph separator
   ["\n\\([ \t]*\n\\)+" 0 "\n\n</p>\n\n<p>\n"]

   ;; if table.el was loaded, allow for pretty tables.  otherwise only
   ;; simple table markup is supported, nothing fancy.  use | to
   ;; separate cells, || to separate header elements, and ||| for
   ;; footer elements
   (vector
    (if (featurep 'table)
	"^\\(\\s-*\\)\\(\\+[-+]+\\+[\n\r \t]+|\\)"
      "^\\s-*\\(\\([^|\n]+\\(|+\\)\\s-*\\)+\\)\\([^|\n]+\\)?$")
    1 'emacs-wiki-markup-table)

   ;; unnumbered List items begin with a -.  numbered list items
   ;; begin with number and a period.  definition lists have a
   ;; leading term separated from the body with ::.  centered
   ;; paragraphs begin with at least six columns of whitespace; any
   ;; other whitespace at the beginning indicates a blockquote.  The
   ;; reason all of these rules are handled here, is so that
   ;; blockquote detection doesn't interfere with indented list
   ;; members.
   ["^\\(\\s-*\\(-\\|[0-9]+\\.\\|\\(.+\\)[ \t]+::\n?\\)\\)?\\([ \t]+\\)" 4
    emacs-wiki-markup-list-or-paragraph]

   ;; "verse" text is indicated the same way as a quoted e-mail
   ;; response: "> text", where text may contain initial whitespace
   ;; (see below).
   ["<p>\\s-+> \\(\\([^\n]\n?\\)+\\)\\(\\s-*</p>\\)?" 0
    emacs-wiki-markup-verse]

   ;; join together the parts of a list
   ["</\\([oud]l\\)>\\s-*\\(</p>\\s-*<p>\\s-*\\)?<\\1>\\s-*" 0 ""]

   ;; join together the parts of a table
   (vector
    (concat "</tbody>\\s-*"
	    "</table>\\s-*" "\\(</p>\\s-*<p>\\s-*\\)?" "<table[^>]*>\\s-*"
	    "<tbody>\\s-*") 0 "")
   ["</table>\\s-*\\(</p>\\s-*<p>\\s-*\\)?<table[^>]*>\\s-*" 0 ""]

   ;; fixup paragraph delimiters
   (vector
    (concat "<p>\\s-*\\(</?" emacs-wiki-block-groups-regexp ">\\)") 0 "\\1")
   (vector (concat "\\(</?" emacs-wiki-block-groups-regexp
		   ">\\)\\s-*\\(</p>\\)") 3 "\\1")

   ;; terminate open paragraph at the end of the buffer
   ["<p>\\s-*\\'" 0 ""]
   ;; make sure to close any open text (paragraphs)
   ["\\([^> \t\n]\\)\\s-*\\'" 0 "\\1\n</p>"]
   ;; lists have no whitespace after them, so add a final linebreak
   ["\\(</[oud]l>\\)\\(\\s-*\\(<hr>\\|\\'\\)\\)" 0 "\\1\n<br>\\2"]

   ;; replace WikiLinks in the buffer (links to other pages)
   ;; <nop> before a WikiName guards it from being replaced
   ;; '''' can be used to add suffixes, such as WikiName''''s
   [emacs-wiki-url-or-name-regexp 0 emacs-wiki-markup-link]
   ["''''" 0 ""]

   ;; bare email addresses
   (vector
    (concat
     "\\([^:.@/a-zA-Z0-9]\\)"
     "\\([-a-zA-Z0-9._]+@\\([-a-zA-z0-9_]+\\.\\)+[a-zA-Z0-9]+\\)"
     "\\([^\"a-zA-Z0-9]\\)")
    0
    "\\1<a href=\"mailto:\\2\">\\2</a>\\4")

   ;; replace quotes, since most browsers don't understand `` and ''
   ["\\(``\\|''\\)" 0 "\""]

   ;; insert the default publishing header
   (function
    (lambda ()
      (insert emacs-wiki-publishing-header)))

   ;; insert the default publishing footer
   (function
    (lambda ()
      (goto-char (point-max))
      (insert emacs-wiki-publishing-footer)))

   ;; process any remaining markup tags
   [emacs-wiki-tag-regexp 0 emacs-wiki-markup-custom-tags])
  "List of markup rules to apply when publishing a Wiki page.
Each member of the list is either a function, or a vector of the form:

  [REGEXP/SYMBOL TEXT-BEGIN-GROUP REPLACEMENT-TEXT/FUNCTION/SYMBOL].

REGEXP is a regular expression, or symbol whose value is a regular
expression, which is searched for using `re-search-forward'.
TEXT-BEGIN-GROUP is the matching group within that regexp which
denotes the beginning of the actual text to be marked up.
REPLACEMENT-TEXT is a string that will be passed to `replace-match'.
If it is not a string, but a function, it will be called to determine
what the replacement text should be (it must return a string).  If it
is a symbol, the value of that symbol should be a string.

The replacements are done in order, one rule at a time.  Writing the
regular expressions can be a tricky business.  Note that case is never
ignored.  `case-fold-search' is always be bound to nil while
processing the markup rules.

Here is a description of the default markup rules:

Headings

 * First level
 ** Second level
 *** Third level

 Note that the first level is actually indicated using H2, so that
 it doesn't appear at the same level as the page heading (which
 conceptually titles the section of that Wiki page).

Horizontal rules

----

Emphasis

 *emphasis*
 **strong emphasis**
 ***very strong emphasis***
 _underlined text_
 =verbatim=

 <verbatim>This tag should be used for larger blocks of
 text</verbatim>.

Footnotes

  A reference[1], which is just a number in square brackets,
  constitutes a footnote reference.

  Footnotes:

  [1]  Footnotes are defined by the same number in brackets
       occurring at the beginning of a line.  Use footnote-mode's C-c
       ! a command, to very easily insert footnotes while typing.  Use
       C-x C-x to return to the point of insertion.

Paragraphs

  One or more blank lines separates paragraphs.

Centered paragraphs and quotations

  A line that begins with six or more columns of whitespace (made up
  of tabs or spaces) indicates a centered paragraph.  I assume this
  because it's expected you will use M-s to center the line, which
  usually adds a lot of whitespace before it.

  If a line begins with some whitespace, but less than six columns, it
  indicates a quoted paragraph.

Poetic verse

  Poetry requires that whitespace be preserved, without resorting to
  the monospace typical of <pre>.  For this, the following special
  markup exists, which is reminiscent of e-mail quotations:

    > A line of Emacs verse;
    > forgive its being so terse.

  You can also use the <verse> tag, if you prefer:

    <verse>
    A line of Emacs verse;
    forgive its being so terse.
    </verse>

Literal paragraphs

  Use the HTML tags <pre></pre> to insert a paragraph and preserve
  whitespace.  If you're inserting a block of code, you will almost
  always want to use <verbatim></verbatim> *within* the <pre> tags.
  The shorcut for doing this is to use the <example> tag:

    <example>
    Some literal text or code here.
    </example>

Lists

  - bullet list

  1. Enumerated list

  Term :: A definition list

  Blank lines between list elements are optional, but required between
  members of a definition list.

Tables

  There are two forms of table markup supported.  If Takaaki Ota's
  table.el package is available, then simply create your tables using
  his package, and they will be rendered into the appropriate HTML.

  If table.el is not available, then only very simple table markup is
  supported.  The attributes of the table are kept in
  `emacs-wiki-table-attributes'.  The syntax is:

    Double bars || Separate header fields
    Single bars | Separate body fields
    Here are more | body fields
    Triple bars ||| Separate footer fields

  Other paragraph markup applies to both styles, meaning that if six
  or more columns of whitespace precedes the first line of the table,
  it will be centered, and if any whitespace at all precedes first
  line, it will occur in a blockquote.

Anchors and tagged links

  #example If you begin a line with \"#anchor\" -- where anchor
  can be any word that doesn't contain whitespace -- it defines an
  anchor at that point into the document.  This anchor text is not
  displayed.

  You can reference an anchored point in another page (or even in the
  current page) using WikiName#anchor.  The #anchor will never be
  displayed in HTML, whether at the point of definition or reference,
  but it will cause browsers to jump to that point in the document.

Redirecting to another page or URL

  Sometimes you may wish to redirect someone to another page.  To do
  this, put:

    <redirect url=\"http://somewhereelse.com\"/>

  at the top of the page.  If the <redirect> tag specifies content,
  this will be used as the redirection message, rather than the
  default.

  The numbers of seconds to delay is defined by
  `emacs-wiki-redirect-delay', which defaults to 2 seconds.  The page
  shown will also contain a link to click on, for browsing which do
  not support automatic refreshing.

URLs

  A regular URL is given as a link.  If it's an image URL, it will
  be inlined using an IMG tag.

Embedded lisp

  <lisp>(concat \"This form gets\" \"inserted\")</lisp>

Special handling of WikiNames

  If you need to add a plural at the end of a WikiName, separate it
  with four single quotes: WikiName''''s.

  To prevent a link name (of any type) from being treated as such,
  surround it with =equals= (to display it in monotype), or prefix it
  with the tag <nop>.

Special Wiki links

  Besides the normal WikiName type links, emacs-wiki also supports
  extended links:

    [[link text][optional link description]]

  An extended link is always a link, no matter how it looks.  This
  means you can use any file in your `emacs-wiki-directories' as a
  Wiki file.  If you provide an optional description, that's what will
  be shown instead of the link text.  This is very useful for
  providing textual description of URLs.

  See the documentation to emacs-wiki-image-regexp for how to inline
  files and images.

InterWiki names

  There are times when you will want to constantly reference pages on
  another website.  Rather than repeating the URL ad nauseum, you can
  define an InterWiki name.  This is a set of WikiNames to URL
  correlations, that support textual substitution using #anchor names
  (which are appended to the URL).  For example, MeatballWiki is
  defined in the variable `emacs-wiki-interwiki-names'.  It means you
  can reference the page \"MeatBall\" on MeatballWiki using this
  syntax:

    MeatballWiki#MeatBall

  In the resulting HTML, the link is simply shown as
  \"MeatballWiki:MeatBall\"."
  :type '(repeat
	  (choice
	   (vector :tag "Markup rule"
		   (choice regexp symbol)
		   integer
		   (choice string function symbol))
	   function))
  :group 'emacs-wiki-publish)

(defcustom emacs-wiki-changelog-markup
  (list
   ["&" 0 "&amp;"]
   ["<" 0 "&lt;"]
   [">" 0 "&gt;"]

   ["^\\(\\S-+\\)\\s-+\\(.+\\)" 0 emacs-wiki-markup-changelog-section]

   ;; emphasized or literal text
   ["\\(^\\|[-[ \t\n<('`\"]\\)\\(=[^= \t\n]\\|_[^_ \t\n]\\|\\*+[^* \t\n]\\)"
    2 emacs-wiki-markup-word]

   ;; headings, outline-mode style
   ["^\\*\\s-+\\(.+\\)$" 0 "<h2>\\1</h2>"]

   ;; escape the 'file' entries, incase they are extended wiki links
   ["^[ \t]+\\* \\([^:]+\\):" 0 emacs-wiki-changelog-escape-files]

   ;; don't require newlines between unnumbered lists.
   ["^\\s-*\\(\\*\\)" 1 "\n\\1"]

   ;; the beginning of the buffer begins the first paragraph
   ["\\`\n*" 0 "<p>\n"]
   ;; plain paragraph separator
   ["\n\\([ \t]*\n\\)+" 0 "\n\n</p>\n\n<p>\n"]

   ;; unnumbered List items begin with a -.  numbered list items
   ;; begin with number and a period.  definition lists have a
   ;; leading term separated from the body with ::.  centered
   ;; paragraphs begin with at least six columns of whitespace; any
   ;; other whitespace at the beginning indicates a blockquote.  The
   ;; reason all of these rules are handled here, is so that
   ;; blockquote detection doesn't interfere with indented list
   ;; members.
   ["^\\(\\s-*\\(\\*\\)\\)?\\([ \t]+\\)\\(\\([^\n]\n?\\)+\\)" 3
    "<ul>\n<li>\\4</ul>\n"]

   ;; join together the parts of a list
   ["</\\([oud]l\\)>\\s-*\\(</p>\\s-*<p>\\s-*\\)?<\\1>\\s-*" 0 ""]

   ;; fixup paragraph delimiters
   (vector
    (concat "<p>\\s-*\\(</?" emacs-wiki-block-groups-regexp ">\\)") 0 "\\1")
   (vector (concat "\\(</?" emacs-wiki-block-groups-regexp
		   ">\\)\\s-*\\(</p>\\)") 3 "\\1")

   ;; terminate open paragraph at the end of the buffer
   ["<p>\\s-*\\'" 0 ""]
   ;; make sure to close any open text (paragraphs)
   ["\\([^> \t\n]\\)\\s-*\\'" 0 "\\1\n</p>"]
   ;; lists have no whitespace after them, so add a final linebreak
   ["\\(</[oud]l>\\)\\(\\s-*\\(<hr>\\|\\'\\)\\)" 0 "\\1\n<br>\\2"]

   ;; bare email addresses
   (vector
    (concat
     "\\([^:.@/a-zA-Z0-9]\\)"
     "\\([-a-zA-Z0-9._]+@\\([-a-zA-z0-9_]+\\.\\)+[a-zA-Z0-9]+\\)"
     "\\([^\"a-zA-Z0-9]\\)")
    0
    "\\1<a href=\"mailto:\\2\">\\2</a>\\4")

   ;; replace WikiLinks in the buffer (links to other pages)
   [emacs-wiki-url-or-name-regexp 0 emacs-wiki-markup-link]
   ["''''" 0 ""]

   ;; insert the default publishing header
   (function
    (lambda ()
      (insert emacs-wiki-publishing-header)))

   ;; insert the default publishing footer
   (function
    (lambda ()
      (goto-char (point-max))
      (insert emacs-wiki-publishing-footer)))

   ;; process any remaining markup tags
   [emacs-wiki-tag-regexp 0 emacs-wiki-markup-custom-tags])
  "List of markup rules for publishing ChangeLog files.
These are used when the wiki page's name is ChangeLog."
  :type '(repeat
	  (choice
	   (vector :tag "Markup rule"
		   (choice regexp symbol)
		   integer
		   (choice string function symbol))
	   function))
  :group 'emacs-wiki-publish)

(defun emacs-wiki-transform-content-type (content-type)
  "Using `emacs-wiki-coding-map', try and resolve an emacs coding
  system to an associated HTML coding system. If no match is found,
  `emacs-wiki-charset-default' is used instead."
  (let ((match (assoc (coding-system-base content-type)
                      emacs-wiki-coding-map)))
    (if match
        (cadr match)
      emacs-wiki-charset-default)))

(defun emacs-wiki-private-p (name)
  "Return non-nil if NAME is a private page, and shouldn't be published."
  (if name
      (if emacs-wiki-use-mode-flags
	  (let* ((page-file (emacs-wiki-page-file name t))
		 (filename (and page-file (file-truename page-file))))
	    (if filename
		(or (eq ?- (aref (nth 8 (file-attributes
					 (file-name-directory filename))) 7))
		    (eq ?- (aref (nth 8 (file-attributes filename)) 7)))))
	(let ((private-pages emacs-wiki-private-pages) private)
	  (while private-pages
	    (if (string-match (car private-pages) name)
		(setq private t private-pages nil)
	      (setq private-pages (cdr private-pages))))
	  private))))

(defun emacs-wiki-editable-p (name)
  "Return non-nil if NAME is a page that may be publically edited.
If the page does not exist, the page will be created if: mode flags
are not being checked, and it is a page listed in
`emacs-wiki-editable-pages', or the first directory in
`emacs-wiki-directories' is writable.  In either case, the new page
will be created in the first directory in `emacs-wiki-directories'."
  (if (and name emacs-wiki-http-support-editing)
      (if emacs-wiki-use-mode-flags
	  (let ((filename
		 (file-truename
		  (or (emacs-wiki-page-file name t)
		      (expand-file-name name (car emacs-wiki-directories))))))
	    (if (file-exists-p filename)
		(eq ?w (aref (nth 8 (file-attributes filename)) 8))
	      (eq ?w (aref (nth 8 (file-attributes
				   (file-name-directory filename))) 8))))
	(let ((editable-pages emacs-wiki-editable-pages) editable)
	  (while editable-pages
	    (if (string-match (car editable-pages) name)
		(setq editable t editable-pages nil)
	      (setq editable-pages (cdr editable-pages))))
	  editable))))

(defun emacs-wiki-visit-published-file (&optional arg)
  "Visit the current wiki page's published result."
  (interactive "P")
  (if arg
      (find-file-other-window (emacs-wiki-published-file))
    (funcall emacs-wiki-browse-url-function
	     (concat "file:" (emacs-wiki-published-file)))))

(defun emacs-wiki-dired-publish ()
  "Publish all marked files in a dired buffer."
  (interactive)
  (emacs-wiki-publish-files (dired-get-marked-files) t))

(defun emacs-wiki-prettify-title (title)
  "Prettify the given TITLE."
  (save-match-data
    (let ((case-fold-search nil))
      (while (string-match "\\([A-Za-z]\\)\\([A-Z0-9]\\)" title)
	(setq title (replace-match "\\1 \\2" t nil title)))
      (let* ((words (split-string title))
	     (w (cdr words)))
	(while w
	  (if (member (downcase (car w))
		      emacs-wiki-downcase-title-words)
	      (setcar w (downcase (car w))))
	  (setq w (cdr w)))
	(mapconcat 'identity words " ")))))

(defun emacs-wiki-publish (&optional arg)
  "Publish all wikis that need publishing.
If the published wiki already exists, it is only overwritten if the
wiki is newer than the published copy.  When given the optional
argument ARG, all wikis are rewritten, no matter how recent they are.
The index file is rewritten no matter what."
  (interactive "P")
  ;; prompt to save any emacs-wiki buffers
  (save-some-buffers nil (lambda ()
                           (eq major-mode 'emacs-wiki-mode)))
  ;; ensure the publishing location is available
  (unless (file-exists-p emacs-wiki-publishing-directory)
    (message "Creating publishing directory %s"
             emacs-wiki-publishing-directory)
    (make-directory emacs-wiki-publishing-directory))
  (if (emacs-wiki-publish-files
       (let* ((names (emacs-wiki-file-alist))
	      (files (list t))
	      (lfiles files))
	 (while names
	   (setcdr lfiles (cons (cdar names) nil))
	   (setq lfiles (cdr lfiles)
		 names (cdr names)))
	 (cdr files)) arg)
      ;; republish the index if any pages were published
      (with-current-buffer (emacs-wiki-generate-index t t)
	(emacs-wiki-replace-markup emacs-wiki-index-page)
	(let ((backup-inhibited t))
	  (write-file (emacs-wiki-published-file emacs-wiki-index-page)))
	(kill-buffer (current-buffer))
	(message "All Wiki pages%s have been published."
		 (if emacs-wiki-current-project
		     (concat " for project " emacs-wiki-current-project)
		   "")))
    (message "No Wiki pages%s need publishing at this time."
	     (if emacs-wiki-current-project
		 (concat " in project " emacs-wiki-current-project)
	       ""))))

(defun emacs-wiki-publish-this-page ()
  "Force publication of the current page."
  (interactive)
  (emacs-wiki-publish-files (list buffer-file-name) t))

(defun emacs-wiki-publish-files (files force)
  "Publish all files in list FILES.
If the argument FORCE is nil, each file is only published if it is
newer than the published version.  If the argument FORCE is non-nil,
the file is published no matter what."
  (let (published-some file page published)
    (while files
      (setq file (car files)
	    files (cdr files)
	    page (emacs-wiki-page-name file)
            published (emacs-wiki-published-file page))
      (if (and (not (emacs-wiki-private-p page))
	       (or force (file-newer-than-file-p file published)))
	  (with-temp-buffer
	    (insert-file-contents file t)
            (cd (file-name-directory file))
	    (emacs-wiki-maybe)
	    (emacs-wiki-replace-markup)
            (let ((backup-inhibited t)
                  (buffer-file-coding-system
                   (when (boundp 'buffer-file-coding-system)
                     buffer-file-coding-system)))
              (when (eq buffer-file-coding-system 'undecided-unix)
                ;; make it agree with the default charset
                (setq buffer-file-coding-system
                      emacs-wiki-coding-default))
              (write-file published))
            (setq published-some t))))
    published-some))



(defun emacs-wiki-escape-html-specials (&optional end)
  (while (and (or (< (point) end) (not end))
              (re-search-forward "[<>&\"]" end t))
    (cond
     ((eq (char-before) ?\")
      (delete-char -1)
      (insert "&quot;"))
     ((eq (char-before) ?\<)
      (delete-char -1)
      (insert "&lt;"))
     ((eq (char-before) ?\>)
      (delete-char -1)
      (insert "&gt;"))
     ((eq (char-before) ?\&)
      (delete-char -1)
      (insert "&amp;")))))

;; we currently only do this on links. this means a stray '&' in an
;; emacs-wiki document risks being misinterpreted when being published, but
;; this is the price we pay to be able to inline HTML content without special
;; tags.
(defun emacs-wiki-escape-html-string (str)
  "Convert to character entities any non alphanumeric characters outside of a
  few punctuation symbols, that risk being misinterpreted if not escaped"
  (when str
    (let (pos code len)
      (save-match-data
        (while (setq pos (string-match "[^-[:alnum:]/:._=@\\?~#]" str pos))
          (setq code (int-to-string (aref str pos)))
          (setq len (length code))
          (setq str (replace-match (concat "&#" code ";") nil nil str))
          (setq pos (+ 3 len pos)))
        str))))

(defun emacs-wiki-replace-markup (&optional title)
  "Replace markup according to `emacs-wiki-publishing-markup'."
  (let* ((emacs-wiki-meta-http-equiv emacs-wiki-meta-http-equiv)
	 (emacs-wiki-current-page-title title)
	 (emacs-wiki-publishing-p t)
	 (case-fold-search nil)
	 (inhibit-read-only t)
	 (rules (if (string= (emacs-wiki-page-name) "ChangeLog")
		    emacs-wiki-changelog-markup
		  emacs-wiki-publishing-markup))
	 (limit (* (length rules) (point-max)))
	 (verbose (and emacs-wiki-report-threshhold
		       (> (point-max) emacs-wiki-report-threshhold)))
	 (base 0)
         (emacs-wiki-meta-content
	  (concat emacs-wiki-meta-content-type "; charset="
                  (if (stringp emacs-wiki-meta-content-coding)
                      emacs-wiki-meta-content-coding
                    (emacs-wiki-transform-content-type
                     (or buffer-file-coding-system
                         emacs-wiki-coding-default))))))
    (run-hooks 'emacs-wiki-before-markup-hook)
    (while rules
      (goto-char (point-min))
      (if (functionp (car rules))
	  (funcall (car rules))
	(let ((regexp (aref (car rules) 0))
	      (group (aref (car rules) 1))
	      (replacement (aref (car rules) 2))
	      start last-pos pos)
	  (if (symbolp regexp)
	      (setq regexp (symbol-value regexp)))
	  (if verbose
	      (message "Publishing %s...%d%%"
		       (emacs-wiki-page-name)
		       (* (/ (float (+ (point) base)) limit) 100)))
	  (while (and regexp (setq pos (re-search-forward regexp nil t)))
	    (if verbose
		(message "Publishing %s...%d%%"
			 (emacs-wiki-page-name)
			 (* (/ (float (+ (point) base)) limit) 100)))
	    (unless (get-text-property (match-beginning group) 'read-only)
	      (let ((text (cond
			   ((functionp replacement)
			    (funcall replacement))
			   ((symbolp replacement)
			    (symbol-value replacement))
			   (t replacement))))
		(when text
                  (condition-case nil
                      (replace-match text t)
                    (error
                     (replace-match "[FIXME: invalid characters]" t))))))
                (if (and last-pos (= pos last-pos))
                    (if (eobp)
                        (setq regexp nil)
                      (forward-char 1)))
                (setq last-pos pos))))
          (setq rules (cdr rules)
                base (+ base (point-max))))
        (run-hooks 'emacs-wiki-after-markup-hook)
        (if verbose
            (message "Publishing %s...done" (emacs-wiki-page-name)))))

(defun emacs-wiki-custom-tags (&optional highlight-p)
  (let ((tag-info (or (assoc (match-string 1) emacs-wiki-markup-tags)
		      (assoc (match-string 1) emacs-wiki-dangerous-tags))))
    (when (and tag-info (or (not highlight-p)
			    (nth 3 tag-info)))
      (let ((closed-tag (match-string 3))
	    (start (match-beginning 0))
	    (beg (point)) end attrs)
	(when (nth 2 tag-info)
	  (let ((attrstr (match-string 2)))
	    (while (and attrstr
			(string-match
			 "\\([^ \t\n=]+\\)\\(=\"\\([^\"]+\\)\"\\)?" attrstr))
	      (let ((attr (cons (downcase
				 (match-string-no-properties 1 attrstr))
				(match-string-no-properties 3 attrstr))))
		(setq attrstr (replace-match "" t t attrstr))
		(if attrs
		    (nconc attrs (list attr))
		  (setq attrs (list attr)))))))
	(if (and (cadr tag-info) (not closed-tag))
	    (if (search-forward (concat "</" (car tag-info) ">") nil t)
		(unless highlight-p
		  (delete-region (match-beginning 0) (point)))
	      (setq tag-info nil)))
	(when tag-info
	  (setq end (point-marker))
	  (unless highlight-p
	    (delete-region start beg))
	  (goto-char (if highlight-p beg start))
	  (let ((args (list start end)))
	    (if (nth 2 tag-info)
		(nconc args (list attrs)))
	    (if (nth 3 tag-info)
		(nconc args (list highlight-p)))
	    (apply (nth 4 tag-info) args))))))
  nil)

(defun emacs-wiki-markup-initial-directives ()
  (cond
   ((string= (match-string 1) "title")
    (set (make-local-variable 'emacs-wiki-current-page-title) (match-string 2)))
   (t ;; "style"
    (set (make-local-variable 'emacs-wiki-style-sheet)
	  (concat "<link rel=\"stylesheet\" type=\"text/css\" href=\""
		  (match-string 2) "\">"))))
  "")

(defalias 'emacs-wiki-markup-custom-tags 'emacs-wiki-custom-tags)

(defun emacs-wiki-highlight-title ()
  (add-text-properties (+ 7 (match-beginning 0))
                       (line-end-position)
                       '(face emacs-wiki-header-1)))

(defun emacs-wiki-highlight-custom-tags ()
  ;; Remove the match-data related to the url-or-name-regexp, which is
  ;; part of emacs-wiki-highlight-regexp.  All in the name of speed.
  (let ((match-data (match-data)))
    (setcdr (cdr match-data)
	    (nthcdr (* 2 (+ 2 emacs-wiki-url-or-name-regexp-group-count))
		    match-data))
    (set-match-data match-data)
    (emacs-wiki-custom-tags t)))

(defun emacs-wiki-example-tag (beg end highlight-p)
  (if highlight-p
      (progn
        (emacs-wiki-multiline-maybe beg end)
        (goto-char end))
    (insert "<pre class=\"example\">")
    (emacs-wiki-escape-html-specials end)
    (when (< (point) end)
      (goto-char end))
    (insert "</pre>")
    (add-text-properties beg (point) '(rear-nonsticky (read-only)
                                                      read-only t))))

(defun emacs-wiki-verbatim-tag (beg end highlight-p)
  (if highlight-p
      (progn
        (emacs-wiki-multiline-maybe beg end)
        (goto-char end))
    (emacs-wiki-escape-html-specials end)
    (add-text-properties beg end '(rear-nonsticky (read-only)
						  read-only t))))

(defun emacs-wiki-nowiki-tag (beg end highlight-p)
  (if highlight-p
      (goto-char end)
    (add-text-properties
     beg end '(read-nonsticky (read-only) read-only t))))

(defun emacs-wiki-verse-tag (beg end)
  (save-excursion
    (while (< (point) end)
      (unless (eq (char-after) ?\n)
	(insert "> "))
      (forward-line))))

(defvar emacs-wiki-numbered-counter 1)
(make-variable-buffer-local 'emacs-wiki-numbered-counter)

(defun emacs-wiki-numbered-tag (beg end)
  (save-excursion
    (goto-char beg)
    (setq end (copy-marker (1- end)))
    (insert "<table cellspacing=\"8\">")
    (insert (format "<tr><td valign=\"top\"><strong>%d</strong></td>
<td><p><a name=\"%d\"/>" emacs-wiki-numbered-counter
			 emacs-wiki-numbered-counter))
    (setq emacs-wiki-numbered-counter
	  (1+ emacs-wiki-numbered-counter))
    (while (and (< (point) end)
		(re-search-forward "^$" end t))
      (replace-match (format "</p>
</td>
</tr><tr><td valign=\"top\"><strong>%d</strong></td><td>
<p><a name=\"%d\"/>" emacs-wiki-numbered-counter
		     emacs-wiki-numbered-counter))
      (setq emacs-wiki-numbered-counter
	    (1+ emacs-wiki-numbered-counter)))
    (goto-char end)
    (insert (format "</p>
</td></tr></table>" (1+ emacs-wiki-numbered-counter)))))

(defun emacs-wiki-redirect-tag (beg end attrs)
  (let ((link (cdr (assoc "url" attrs))))
    (when link
      (setq emacs-wiki-meta-http-equiv "Refresh"
	    emacs-wiki-meta-content
	    (concat (or (cdr (assoc "delay" attrs))
			(int-to-string emacs-wiki-redirect-delay))
		    ";\nURL=" (emacs-wiki-link-url link)))
      (if (= beg end)
	  (insert "You should momentarily be redirected to [[" link "]].")
	(goto-char end))
      (delete-region (point) (point-max)))))

(defun emacs-wiki-nop-tag (beg end highlight-p)
  (if highlight-p
      (add-text-properties beg (point) '(invisible t intangible t)))
  (when (looking-at emacs-wiki-name-regexp)
    (goto-char (match-end 0))
    (unless highlight-p
      (add-text-properties beg (point)
			   '(rear-nonsticky (read-only) read-only t)))))

(defun emacs-wiki-insert-anchor (anchor)
  "Insert an anchor, either around the word at point, or within a tag."
  (skip-chars-forward " \t\n")
  (if (looking-at "<\\([^ />]+\\)>")
      (let ((tag (match-string 1)))
	(goto-char (match-end 0))
	(insert "<a name=\"" anchor "\" id=\"" anchor "\">")
        (when emacs-wiki-anchor-on-word
          (or (and (search-forward (format "</%s>" tag)
                                   (line-end-position) t)
                   (goto-char (match-beginning 0)))
              (forward-word 1)))
	(insert "</a>"))
    (insert "<a name=\"" anchor "\" id=\"" anchor "\">")
    (when emacs-wiki-anchor-on-word
      (forward-word 1))
    (insert "</a>")))

(defun emacs-wiki-contents-tag (beg end attrs)
  (let ((max-depth (let ((depth (cdr (assoc "depth" attrs))))
		     (or (and depth (string-to-int depth)) 2)))
	(index 1)
	base contents l)
    (save-excursion
      (catch 'done
	(while (re-search-forward "^\\(\\*+\\)\\s-+\\(.+\\)" nil t)
	  (setq l (length (match-string 1)))
	  (if (null base)
	      (setq base l)
	    (if (< l base)
		(throw 'done t)))
	  (when (<= l max-depth)
	    (setq contents (cons (cons l (match-string-no-properties 2))
				 contents))
	    (goto-char (match-beginning 2))
	    (emacs-wiki-insert-anchor (concat "sec" (int-to-string index)))
	    (setq index (1+ index))))))
    (setq index 1 contents (reverse contents))
    (let ((depth 1) (sub-open 0) (p (point)))
      (insert "<dl class=\"contents\">\n")
      (while contents
	(insert "<dt class=\"contents\">\n")
	(insert "<a href=\"#sec" (int-to-string index) "\">"
                (cdar contents)
                "</a>\n")
	(setq index (1+ index))
	(insert "</dt>\n")
	(setq depth (caar contents)
	      contents (cdr contents))
	(if contents
	    (cond
	     ((< (caar contents) depth)
	      (let ((idx (caar contents)))
		(while (< idx depth)
		  (insert "</dl>\n</dd>\n")
		  (setq sub-open (1- sub-open)
			idx (1+ idx)))))
	     ((> (caar contents) depth)	; can't jump more than one ahead
	      (insert "<dd>\n<dl class=\"contents\">\n")
	      (setq sub-open (1+ sub-open))))))
      (while (> sub-open 0)
	(insert "</dl>\n</dd>\n")
	(setq sub-open (1- sub-open)))
      (insert "</dl>\n")
      (put-text-property p (point) 'read-only t))))

(defun emacs-wiki-lisp-tag (beg end highlight-p)
  (if highlight-p
      (add-text-properties
       beg end
       (list 'font-lock-multiline t
	     'display (emacs-wiki-eval-lisp
		       (buffer-substring-no-properties (+ beg 6) (- end 7)))
	     'intangible t))
    (save-excursion
      (insert (emacs-wiki-eval-lisp
	       (prog1
		   (buffer-substring-no-properties beg end)
		 (delete-region beg end)))))))

(defcustom emacs-wiki-command-default-file nil
  "If non-nil, this default program to use with <command> tags.
If nil, Eshell is used, since it works on all platforms."
  :type '(choice file (const :tag "Use Eshell" nil))
  :group 'emacs-wiki-publish)

(defun emacs-wiki-command-tag (beg end attrs &optional highlight-p pre-tags)
  (if highlight-p
      (goto-char end)
    (while (looking-at "\\s-*$")
      (forward-line))
    (let ((interp (or (cdr (assoc "file" attrs))
		      emacs-wiki-command-default-file)))
      (if (null interp)
	  (eshell-command (prog1
			      (buffer-substring-no-properties (point) end)
			    (delete-region beg end)) t)
	(let ((file (make-temp-file "ewiki")))
	  (unwind-protect
	      (let ((args (split-string interp)))
		(write-region (point) end file)
		(delete-region beg end)
		(if pre-tags
		    (insert "<pre>\n"))
		(apply 'call-process (car args) file t nil (cdr args))
		(while (eq (char-syntax (char-before)) ? )
		  (backward-char))
		(add-text-properties beg (point)
				     '(rear-nonsticky (read-only)
						      read-only t))
		(if pre-tags
		    (insert "</pre>\n")))
	    (if (file-exists-p file)
		(delete-file file))))))))

(defcustom emacs-wiki-c-to-html
  (if (or (featurep 'executable)
	  (load "executable" t t))
      (concat (executable-find "c2html") " -c -s"))
  "Program to use to convert <c-source> tag text to HTML."
  :type 'string
  :group 'emacs-wiki-publish)

(defun emacs-wiki-c-source-tag (beg end attrs highlight-p)
  (if highlight-p
      (goto-char end)
    (if emacs-wiki-c-to-html
	(let ((c-to-html emacs-wiki-c-to-html))
	  (if (assoc "numbered" attrs)
	      (setq c-to-html (concat c-to-html " -n")))
	  (emacs-wiki-command-tag beg end (list (cons "file" c-to-html))))
      (insert "<pre>")
      (emacs-wiki-escape-html-specials end)
      (goto-char end)
      (add-text-properties beg (point)
			   '(rear-nonsticky (read-only) read-only t))
      (insert "</pre>"))))

(defun emacs-wiki-python-tag (beg end attrs highlight-p)
  (emacs-wiki-command-tag
   beg end (list (cons "file" (executable-find "python"))) highlight-p t))

(defun emacs-wiki-perl-tag (beg end attrs highlight-p)
  (emacs-wiki-command-tag
   beg end (list (cons "file" (executable-find "perl"))) highlight-p t))

(defun emacs-wiki-insert-xbel-bookmarks (bmarks folder)
  "Insert a set of XBEL bookmarks as an HTML list."
  (while bmarks
    (let ((bookmark (car bmarks)))
      (cond
       ((equal (xml-tag-name bookmark) "folder")
	(let ((title (cadr (xml-tag-child bookmark "title"))))
	  (unless folder
	    (insert "<li>" title "\n<ul>\n"))
	  (emacs-wiki-insert-xbel-bookmarks (xml-tag-children bookmark)
					    (if (equal folder title)
						nil
					      folder))
	  (unless folder
	    (insert "</ul>\n"))))
       ((equal (xml-tag-name bookmark) "bookmark")
	(unless folder
	  (insert "<li><a href=\"" (xml-tag-attr bookmark "href") "\">"
		  (cadr (xml-tag-child bookmark "title")) "</a>\n")))))
    (setq bmarks (cdr bmarks))))

(defcustom emacs-wiki-xbel-bin-directory "/usr/bin"
  "Directory where the xbel parsing utilities reside."
  :type 'directory
  :group 'emacs-wiki-publish)

(defun emacs-wiki-bookmarks-tag (beg end attrs)
  (require 'xml-parse)
  (let ((filename (expand-file-name (cdr (assoc "file" attrs))))
	(type (cdr (assoc "type" attrs)))
	(folder (cdr (assoc "folder" attrs)))
	(this-buffer (current-buffer))
	buffer)
    (when filename
      (cond
       (type
	(setq buffer (get-buffer-create " *xbel_parse*"))
	(with-current-buffer buffer
	  (erase-buffer)
	  (call-process
	   (format "%s/%s_parse"
		   (directory-file-name emacs-wiki-xbel-bin-directory) type)
	   nil t nil filename)))
       (t
	(setq buffer (find-file-noselect filename))))
      (insert "<ul>\n")
      (emacs-wiki-insert-xbel-bookmarks
       (with-current-buffer buffer
	 (goto-char (point-min))
	 (when (re-search-forward "<!DOCTYPE\\s-+xbel" nil t) ; XBEL format
	   (goto-char (match-beginning 0))
	   ;; the `cdr' is to skip the "title" child
	   (cdr (xml-tag-children (read-xml))))) folder)
      (insert "</ul>\n")
      (kill-buffer buffer)))
  (while (eq (char-syntax (char-before)) ? )
    (backward-char))
  (add-text-properties beg (point)
		       '(rear-nonsticky (read-only) read-only t)))

(defun emacs-wiki-link-url (wiki-link)
  "Resolve the given WIKI-LINK into its ultimate URL form."
  (let ((link (emacs-wiki-wiki-link-target wiki-link)))
    (save-match-data
      (if (or (emacs-wiki-wiki-url-p link)
	      (string-match emacs-wiki-image-regexp link)
	      (string-match emacs-wiki-file-regexp link))
	  link
	(if (assoc (emacs-wiki-wiki-base link)
		   (emacs-wiki-file-alist t))
	    (if (string-match "#" link)
		(concat (emacs-wiki-published-name
			 (substring link 0 (match-beginning 0))
                         (emacs-wiki-page-name)) "#"
			(substring link (match-end 0)))
	      (emacs-wiki-published-name link (emacs-wiki-page-name))))))))

(defsubst emacs-wiki-link-href (url name)
  "Return an href string for URL and NAME."
  (concat "<a href=\"" (emacs-wiki-published-name url) "\">" name "</a>"))

(defun emacs-wiki-markup-link ()
  "Resolve the matched wiki-link into its ultimate <a href> form.
Images used the <img> tag."
  ;; avoid marking up urls that appear to be inside existing HTML
  (when (and (not (eq (char-after (point)) ?\"))
             (not (eq (char-after (point)) ?\>)))
    (let* ((wiki-link (match-string 0))
           (url (emacs-wiki-escape-html-string
                 (emacs-wiki-link-url wiki-link)))
           (name (emacs-wiki-escape-html-string
                  (emacs-wiki-wiki-visible-name wiki-link))))
      (if (null url)
          (if (and emacs-wiki-serving-p
                   (emacs-wiki-editable-p (emacs-wiki-wiki-base wiki-link)))
              (concat "<a class=\"nonexistent\" href=\"editwiki?"
                      (emacs-wiki-wiki-base wiki-link) "\">" name "</a>")
            (concat "<a class=\"nonexistent\" href=\""
                    emacs-wiki-maintainer "\">" name "</a>"))
        (if (save-match-data
              (string-match emacs-wiki-image-regexp url))
            (concat "<img src=\"" url "\" alt=\"" name "\">")
          (concat "<a href=\"" url "\">" name "</a>"))))))

(defun emacs-wiki-markup-word ()
  (let* ((beg (match-beginning 2))
	 (end (1- (match-end 2)))
	 (leader (buffer-substring-no-properties beg end))
	 open-tag close-tag mark-read-only loc multi-line)
    (cond
     ((string= leader "_")
      (setq open-tag "<u>" close-tag "</u>"))
     ((string= leader "=")
      (setq open-tag "<code>" close-tag "</code>")
      (setq mark-read-only t))
     (t
      (setq multi-line t)
      (let ((l (length leader)))
	(cond
	 ((= l 1) (setq open-tag "<em>" close-tag "</em>"))
	 ((= l 2) (setq open-tag "<strong>" close-tag "</strong>"))
	 ((= l 3) (setq open-tag "<strong><em>"
			close-tag "</em></strong>"))))))
    (if (and (setq loc (search-forward leader nil t))
             (eq 0 (skip-syntax-forward "w" (1+ loc)))
             (or multi-line (= 1 (count-lines beg loc))))
        (progn
          (replace-match "")
          (insert close-tag)
          (save-excursion
            (goto-char beg)
            (delete-region beg end)
            (insert open-tag))
          (if mark-read-only
              (add-text-properties beg (point)
                                   '(rear-nonsticky (read-only) read-only
                                   t))))
      (backward-char))
    nil))

(defun emacs-wiki-markup-anchor ()
  (save-match-data
    (emacs-wiki-insert-anchor (match-string 1)))
  "")

(defcustom emacs-wiki-entity-table
  '(("#7779" . "s")
    ("#7717" . "h")
    ("#7789" . "t")
    ("#7716" . "H")
    ("#7826" . "Z"))
  "Substitutions to use for HTML entities which are not fully
supported by all browsers -- in other words, we are pre-empting the
entity mechanism and providing our own textual equivalent.  For
Unicode browsers, this is usually unnecessary."
  :type 'sexp
  :group 'emacs-wiki)

(defun emacs-wiki-markup-entity ()
  (or (cdr (assoc (match-string 1)
		  emacs-wiki-entity-table))
      (concat "&" (match-string 1) ";")))

(defsubst emacs-wiki-surround-text (beg-tag end-tag move-func)
  (insert beg-tag)
  (funcall move-func)
  (insert end-tag))			; returns nil for us

(defun emacs-wiki-markup-heading ()
  (let ((len (1+ (length (match-string 1)))))
    (emacs-wiki-surround-text (format "<h%d>" len) (format "</h%d>" len)
			      'end-of-line)
    ""))

(defun emacs-wiki-markup-footnote ()
  (if (/= (line-beginning-position) (match-beginning 0))
      "<sup><a name=\"fnr.\\1\" href=\"#fn.\\1\">\\1</a></sup>"
    (prog1
	"<sup>[<a name=\"fn.\\1\" href=\"#fnr.\\1\">\\1</a>]</sup>"
      (save-excursion
	(save-match-data
	  (let* ((beg (goto-char (match-end 0)))
		 (end (and (search-forward "\n\n" nil t)
			   (prog1
			       (copy-marker (match-beginning 0))
			     (goto-char beg)))))
	    (while (re-search-forward "^[ \t]+\\([^\n]\\)" end t)
	      (replace-match "\\1" t))))))))

(defsubst emacs-wiki-forward-paragraph ()
  (and (re-search-forward "^\\s-*$" nil t)
       (match-beginning 0)))

(defun emacs-wiki-markup-list-or-paragraph ()
  "Markup a list entry or quoted paragraph.
The reason this function is so funky, is to prevent text properties
like read-only from being inadvertently deleted."
  (if (null (match-string 2))
      (let* ((ws (match-string 4))
	     (tag (if (>= (string-width ws) 6)
		      "center"
		    "blockquote")))
	(unless (and (equal tag "blockquote")
		     (save-excursion
		       (forward-line)
		       (or (eolp)
			   (looking-at "\\S-"))))
	  (emacs-wiki-surround-text (format "<%s>\n<p>\n%s" tag ws)
				    (format "\n</p>\n</%s>\n" tag)
				    'emacs-wiki-forward-paragraph)))
    (let ((str (match-string 2)))
      (cond
       ((and (eq (aref str 0) ?-))
	(delete-region (match-beginning 0) (match-end 0))
	(emacs-wiki-surround-text
	 "<ul>\n<li>" "</li>\n</ul>\n"
	 (function
	  (lambda ()
	    (and (re-search-forward "^\\s-*\\(-\\|$\\)" nil t)
		 (goto-char (match-beginning 0)))))))
       ((and (>= (aref str 0) ?0)
	     (<= (aref str 0) ?9))
	(delete-region (match-beginning 0) (match-end 0))
	(emacs-wiki-surround-text
	 "<ol>\n<li>" "</li>\n</ol>\n"
	 (function
	  (lambda ()
	    (and (re-search-forward "^\\s-*\\([0-9]+\\.\\|$\\)" nil t)
		 (goto-char (match-beginning 0)))))))
       (t
	(goto-char (match-beginning 0))
	(insert "<dl>\n<dt>")
	(save-match-data
	  (when (re-search-forward "[ \t\n]+::[ \t\n]+" nil t)
	    (replace-match "</dt>\n<dd>\n")))
	(emacs-wiki-forward-paragraph)
	(insert "</dd>\n</dl>\n"))))))

(defun emacs-wiki-markup-table ()
  (if (featurep 'table)
      (let ((leader (match-string 1))
	    (begin (copy-marker (match-beginning 0)))
	    table end)
	(goto-char (match-end 0))
	(setq table
	      (with-current-buffer (table-generate-source 'html)
		(prog1
		    (buffer-string)
		  (kill-buffer (current-buffer)))))
	(goto-char begin)
	(if (re-search-backward "<p>[ \t\n\r]+" nil t)
	    (replace-match (if (>= (string-width leader) 6)
			       "<center>\n"
			     (if (> (length leader) 0)
				 "<blockquote>\n"
			       ""))))
	(delete-region begin (re-search-forward "-+\\+\\s-*[\r\n]+\\s-*$"
						nil t))
	(insert table)
	(setq end (point-marker))
	(goto-char begin)
	(while (< (point) end)
	  (if (looking-at "^\\s-+")
	      (replace-match ""))
	  (forward-line))
	(goto-char end)
	(if (re-search-forward "[ \t\n\r]+</p>" nil t)
	    (replace-match (if (>= (string-width leader) 6)
			       "\n</center>"
			     (if (> (length leader) 0)
				 "\n</blockquote>"
			       ""))))
	(set-match-data (list begin begin begin begin))
	nil)
    (let* ((str (save-match-data
                  (if (featurep 'xemacs)
                      ;; more emacs divergence. :(
                      (replace-in-string (match-string 1) " *|+ *$" "")
                    (match-string 1))))
           (fields
            (append (save-match-data
                      (split-string str "[ \t]*|+[ \t]*"))
		    (list (match-string 4))))
	   (len (length (match-string 3)))
	   (row (cond ((= len 1) "tbody")
		      ((= len 2) "thead")
		      ((= len 3) "tfoot")))
	   (col (cond ((= len 1) "td")
		      ((= len 2) "th")
		      ((= len 3) "td"))))
      (concat "<table " emacs-wiki-table-attributes ">\n"
	      "<" row ">\n" "<tr>\n<" col ">"
	      (mapconcat 'identity fields (format "</%s><%s>" col col))
	      "</" col ">\n" "</tr>\n" "</" row ">\n"
	      "</table>\n"))))

(defun emacs-wiki-markup-verse ()
  (save-match-data
    (let* ((lines (split-string (match-string 1) "\n"))
	   (l lines))
      (while l
	(if (and (> (length (car l)) 2)
		 (string-match "\\`\\s-*> " (car l)))
	    (setcar l (substring (car l) (match-end 0))))
	(setq l (cdr l)))
      (concat "<p class=\"verse\">"
	      (mapconcat 'identity lines "\n") "</p>"))))

(defcustom emacs-wiki-pretty-changelogs nil
  "If non-nil, markup ChangeLog buffers using pretty tables.
This rule requires that a GIF file called \"onepixel.gif\" be in your
publication tree.  Here is a uuencoded version of such a file:

begin 644 onepixel.gif
M1TE&.#EA`0`!`*$``````/___________R'Y!`'__P$`+``````!``$```(\"
$3`$`.P``
`
end"
  :type 'boolean
  :group 'emacs-wiki-publish)

(defun emacs-wiki-changelog-escape-files ()
  (replace-match "[[\\1]]" t nil nil 1))

(defun emacs-wiki-markup-changelog-section ()
  (if (not emacs-wiki-pretty-changelogs)
      "* \\1 \\2"
    (let ((email (match-string 2))
	  (date (match-string 1)))
      (goto-char (match-beginning 0))
      (delete-region (match-beginning 0) (match-end 0))
      (while (eolp)
	(kill-line 1))
      (insert (format "  <TABLE WIDTH=\"100%%\" BORDER=\"0\"
	 CELLSPACING=\"1\" CELLPADDING=\"2\">
    <TR>
      <TD BGCOLOR=\"black\" BACKGROUND=\"onepixel.gif\">
	<TABLE WIDTH=\"100%%\" BORDER=\"0\"
	       CELLPADDING=\"5\" CELLSPACING=\"0\">
	  <TR>
	    <TD ALIGN=\"left\" BGCOLOR=\"b0c4de\" BACKGROUND=\"onepixel.gif\">
	      <FONT COLOR=\"navy\"> <B>%s</B> </FONT>
	    </TD>
	    <TD ALIGN=\"right\" VALIGN=\"bottom\" BGCOLOR=\"b0c4de\"
		BACKGROUND=\"onepixel.gif\">
	      <FONT SIZE=\"2\" COLOR=\"2f4f4f\"> %s </FONT>
	    </TD>
	  </TR>
	  <TR>
	    <TD BGCOLOR=\"fffff0\" COLSPAN=\"2\" BACKGROUND=\"onepixel.gif\">
	      <FONT COLOR=\"black\">
" email date))
      (add-text-properties (match-beginning 0) (point)
			   '(read-only t rear-nonsticky (read-only))))
    (if (re-search-forward "^[0-9]" nil t)
	(goto-char (1- (match-beginning 0)))
      (goto-char (point-max))
      (while (eq (char-before (1- (point))) ?\n)
	(delete-char -1)))
    (let ((here (1- (point))))
      (insert "
	      </FONT>
	    </TD>
	  </TR>
	</TABLE>
      </TD>
    </TR>
  </TABLE>
  <br>")
      (add-text-properties here (point)
			   '(read-only t rear-nonsticky (read-only)))
      nil)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Emacs Wiki HTTP Server (using httpd.el)
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defgroup emacs-wiki-http nil
  "Options controlling the behaviour of the Emacs Wiki HTTP server.

So, you want to run a Wiki server based on Emacs?  It's simple.
First, you will need two other scripts: httpd.el and cgi.el.  Both of
them can be downloaded from Eric Mardsen's page:

  http://www.chez.com/emarsden/downloads/

Once you have those two scripts, you must decide between two different
methods of serving pages directly from Emacs:

* PERSISTED INVOCATION SERVER

This scheme keeps a dedicated Emacs process running, solely for the
purpose of rendering pages.  It has the disadvantage of occupying
virtual memory when no one is requesting pages.  It has the advantage
of being 50 times faster than the next method.

To use the persisted invoctaion server, you must download the Python
script `httpd-serve' from the same website where you downloaded
emacs-wiki:

  http://www.gci-net.com/~johnw/emacs.html

Once you have have downloaded the script, running it is simple:

  ./httpd-serve --daemon --port 8080 --load /tmp/my-emacs-wiki \
      [path to your HTML files]

The file `/tmp/my-emacs-wiki.el' should contain all the customizations
required by your Wiki setup.  This is how the server knows where to
find your pages.  This script MUST contain the following line:

  (load \"emacs-wiki\")

That's it.  You should now be able to access your Wiki repository at
localhost:8080.  Only world-readable will be visible, and only
world-writable can be edited over HTTP.

* AN EMACS SPAWNED PER REQUEST

The old method of serving Wiki pages directly is to spawn an Emacs
invocation for every request.  This has the advantage of being a far
simpler approach, and it doesn't consume memory if no one is
requesting pages.  The disadvantage is that it's hideously slow, and
multiple requests may bog down your machine's supply of virtual
memory.

Anyway, to use this approach, add the following line to your
/etc/inted.conf file:

  8080 stream tcp nowait.10000 nobody /usr/local/bin/emacs-httpd

The emacs-httpd script should look something like this:

  #!/bin/sh
  /usr/bin/emacs -batch --no-init-file --no-site-file \\
      -l httpd -l cgi -l emacs-wiki \\
      --eval \"(setq httpd-document-root emacs-wiki-publishing-directory \\
		    emacs-wiki-maintainer \\\"mailto:joe@where.com\\\")\" \\
      -f httpd-serve 2> /dev/null

Emacs-wiki will now serve pages directly on port 8080.  Note that if
you need to configure any variables in emacs-wiki, you will have to
repeat those configurations in the emacs-httpd script.

Note: If you have the 'stopafter' tool installed, it's a good idea to
put a limit on how much time each Emacs process is allowed.  And if
you want to render planner.el pages, you'll need to make another
modification.  Here is a more complete example:

  #!/bin/sh
  /usr/bin/stopafter 60 KILL /usr/bin/emacs \\
      -batch --no-init-file --no-site-file \\
      -l httpd -l cgi -l emacs-wiki -l planner \\
      --eval \"(progn \\
	 (setq httpd-document-root emacs-wiki-publishing-directory \\
	       emacs-wiki-maintainer \\\"mailto:joe@where.com\\\") \\
	 (planner-update-wiki-project))\" \\
      -f httpd-serve 2> /dev/null"
  :group 'emacs-wiki)

(defcustom emacs-wiki-http-search-form
  "
<form method=\"GET\" action=\"/searchwiki?get\">
  <center>
    Search for: <input type=\"text\" size=\"50\" name=\"q\" value=\"\">
    <input type=\"submit\" value=\"Search!\">
  </center>
</form>\n"
  "The form presenting for doing searches when using httpd.el."
  :type 'string
  :group 'emacs-wiki-http)

(defcustom emacs-wiki-http-support-editing t
  "If non-nil, allow direct editing when serving over httpd.el.
Note that a page can be edited only if it is world-writable and
`emacs-wiki-use-mode-flags' is set, or if it matches one of the
regexps in `emacs-wiki-editable-pages'."
  :type 'boolean
  :group 'emacs-wiki-http)

(defcustom emacs-wiki-http-edit-form
  "
<form method=\"POST\" action=\"/changewiki?post\">
  <textarea name=\"%PAGE%\" rows=\"25\" cols=\"80\">%TEXT%</textarea>
  <center>
    <input type=\"submit\" value=\"Submit changes\">
  </center>
</form>\n"
  "The form presenting for doing edits when using httpd.el."
  :type 'string
  :group 'emacs-wiki-http)

(defun emacs-wiki-http-send-buffer (&optional title modified code
					      msg no-markup)
  "Markup and send the contents of the current buffer via HTTP."
  (unless no-markup (emacs-wiki-replace-markup title))
  (princ "HTTP/1.0 ")
  (princ (or code 200))
  (princ " ")
  (princ (or msg "OK"))
  (princ httpd-line-terminator)
  (princ "Server: emacs-wiki.el/2.26")
  (princ httpd-line-terminator)
  (princ "Connection: close")
  (princ httpd-line-terminator)
  (princ "MIME-Version: 1.0")
  (princ httpd-line-terminator)
  (princ "Date: ")
  (princ (format-time-string "%a, %e %b %Y %T %Z"))
  (princ httpd-line-terminator)
  (princ "From: ")
  (princ (substring emacs-wiki-maintainer 7))
  (when modified
    (princ httpd-line-terminator)
    (princ "Last-Modified: ")
    (princ (format-time-string "%a, %e %b %Y %T %Z" modified)))
  (princ httpd-line-terminator)
  (princ "Content-Type: text/html; charset=iso-8859-1")
  (princ httpd-line-terminator)
  (princ "Content-Length: ")
  (princ (1- (point-max)))
  (princ httpd-line-terminator)
  (princ httpd-line-terminator)
  (princ (buffer-string)))

(defun emacs-wiki-http-reject (title msg &optional annotation)
  (with-temp-buffer
    (insert msg ".\n")
    (if annotation
	(insert annotation "\n"))
    (emacs-wiki-http-send-buffer title nil 404 msg)))

(defvar emacs-wiki-buffer-mtime nil)
(make-variable-buffer-local 'emacs-wiki-buffer-mtime)

(defun emacs-wiki-sort-buffers (l r)
  (let ((l-mtime (with-current-buffer l
		   emacs-wiki-buffer-mtime))
	(r-mtime (with-current-buffer r
		   emacs-wiki-buffer-mtime)))
    (cond
     ((and (null l-mtime) (null r-mtime)) l)
     ((null l-mtime) r)
     ((null r-mtime) l)
     (t (emacs-wiki-time-less-p r-mtime l-mtime)))))

(defun emacs-wiki-winnow-list (entries &optional predicate)
  "Return only those ENTRIES for which PREDICATE returns non-nil."
  (let ((flist (list t))
	valid p)
    (let ((entry entries))
      (while entry
	(if (funcall predicate (car entry))
	    (nconc flist (list (car entry))))
	(setq entry (cdr entry))))
    (cdr flist)))

(defcustom emacs-wiki-max-cache-size 64
  "The number of pages to cache when serving over HTTP.
This only applies if set while running the persisted invocation
server.  See main documentation for the `emacs-wiki-http'
customization group."
  :type 'integer
  :group 'emacs-wiki-http)

(defun emacs-wiki-prune-cache ()
  "If the page cache has become too large, prune it."
  (let* ((buflist (sort (emacs-wiki-winnow-list
			 (buffer-list)
			 (function
			  (lambda (buf)
			    (with-current-buffer buf
			      emacs-wiki-buffer-mtime))))
			'emacs-wiki-sort-buffers))
	 (len (length buflist)))
    (while (> len emacs-wiki-max-cache-size)
      (kill-buffer (car buflist))
      (setq len (1- len)))))

(defun emacs-wiki-render-page (name)
  "Render the wiki page identified by NAME.
When serving from a dedicated Emacs process (see the httpd-serve
script), a maximum of `emacs-wiki-max-cache-size' pages will be cached
in memory to speed up serving time."
  (if (equal name emacs-wiki-index-page)
      (with-current-buffer (emacs-wiki-generate-index t t)
	(emacs-wiki-http-send-buffer "Wiki Index")
	(kill-buffer (current-buffer)))
    (let ((file (and (not (emacs-wiki-private-p name))
		     (cdr (assoc name (emacs-wiki-file-alist)))))
	  (inhibit-read-only t))
      (if (null file)
	  (emacs-wiki-http-reject "Page not found"
				  (format "Wiki page %s not found" name))
	(set-buffer (get-buffer-create file))
	(let ((modified-time (nth 5 (file-attributes file))))
	  (when (or (null emacs-wiki-buffer-mtime)
		    (emacs-wiki-time-less-p emacs-wiki-buffer-mtime
					    modified-time))
	    (erase-buffer)
	    (setq emacs-wiki-buffer-mtime modified-time))
	  (goto-char (point-max))
	  (if (not (bobp))
	      (emacs-wiki-http-send-buffer nil emacs-wiki-buffer-mtime
					   nil nil t)
	    (insert-file-contents file t)
	    (cd (file-name-directory file))
	    (emacs-wiki-maybe)
	    (emacs-wiki-http-send-buffer nil emacs-wiki-buffer-mtime)))
	(set-buffer-modified-p nil)
	(emacs-wiki-prune-cache)))))

(defun emacs-wiki-wikify-search-results (term)
  "Convert the current buffer's grep results into a Wiki form."
  (goto-char (point-max))
  (forward-line -2)
  (delete-region (point) (point-max))
  (goto-char (point-min))
  (kill-line 2)
  (let ((results (list t)))
    (while (re-search-forward "^.+/\\([^/:]+\\):\\s-*[0-9]+:\\(.+\\)" nil t)
      (let ((page (match-string 1)))
	(unless (or (emacs-wiki-private-p page)
		    (string-match emacs-wiki-file-ignore-regexp page))
	  (let ((text (match-string 2))
		(entry (assoc page results)))
	    (if entry
		(nconc (cdr entry) (list text))
	      (nconc results (list (cons page (list text)))))))))
    (delete-region (point-min) (point-max))
    (setq results
	  (sort (cdr results)
		(function
		 (lambda (l r)
		   (string-lessp (car l) (car r))))))
    (while results
      (unless (emacs-wiki-private-p (caar results))
	(insert "[[" (caar results) "]] ::\n  <p>")
	(let ((hits (cdar results)))
	  (while hits
	    (while (string-match "</?lisp>" (car hits))
	      (setcar hits (replace-match "" t t (car hits))))
	    (while (string-match (concat "\\([^*?[/>]\\)\\<\\(" term "\\)\\>")
				 (car hits))
	      (setcar hits (replace-match "\\1<strong>\\2</strong>"
					  t nil (car hits))))
	    (insert "  > <verbatim>" (car hits) "</verbatim>\n")
	    (setq hits (cdr hits))))
	(insert "</p>\n\n"))
      (setq results (cdr results)))))

(defun emacs-wiki-setup-edit-page (page-name)
  (insert "<verbatim>" emacs-wiki-http-edit-form "</verbatim>")
  (goto-char (point-min))
  (search-forward "%PAGE%")
  (replace-match page-name t t)
  (search-forward "%TEXT%")
  (let ((beg (match-beginning 0))
	(file (emacs-wiki-page-file page-name))
	end)
    (delete-region beg (point))
    (when file
      (insert-file-contents file)
      (save-restriction
	(narrow-to-region beg (point))
	(goto-char (point-min))
	(emacs-wiki-escape-html-specials)))))

(defun emacs-wiki-http-changewiki (&optional content)
  "Change the contents of Wiki page, using the results of a POST request."
  (require 'cgi)
  (unless content
    (goto-char (point-min))
    (if (not (re-search-forward "Content-length:\\s-*\\([0-9]+\\)" nil t))
	(emacs-wiki-http-reject "Content-length missing"
				"No Content-length for POST request"
				(concat "Header received was:\n\n<example>"
					(buffer-string) "</example>\n"))
      (let ((content-length (string-to-number (match-string 1))))
	(erase-buffer)
	(read-event)			; absorb the CRLF separator
	(let ((i 0))
	  (while (< i content-length)
	    (insert (read-event))
	    (setq i (1+ i))))))
    (setq content (buffer-string)))
  (when content
    (let* ((result (cgi-decode content))
	   (page (caar result))
	   (text (cdar result))
	   (len (length text))
	   (require-final-newline t)
	   (pos 0) illegal user)
      (if (not (emacs-wiki-editable-p page))
	  (emacs-wiki-http-reject
	   "Editing not allowed"
	   (format "Editing Wiki page %s is not allowed" page))
	(while (and (null illegal)
		    (setq pos (string-match "<\\s-*\\([^> \t]+\\)"
					    text pos)))
	  (setq pos (match-end 0))
	  (if (assoc (match-string 1 text) emacs-wiki-dangerous-tags)
	      (setq illegal (match-string 1 text))))
	(if illegal
	    (emacs-wiki-http-reject
	     "Disallowed tag used"
	     (format "Public use of &lt;%s&gt; tag not allowed" illegal))
	  (emacs-wiki-find-file page)
	  (if (setq user (file-locked-p buffer-file-name))
	      (emacs-wiki-http-reject
	       "Page is locked"
	       (format "The page \"%s\" is currently being edited by %s."
		       page (if (eq user t) (user-full-name) user)))
	    (let ((inhibit-read-only t)
		  (delete-old-versions t))
	      (erase-buffer)
	      (insert (if (eq (aref text (1- len)) ?%)
			  (substring text 0 (1- len))
			text))
	      (goto-char (point-min))
	      (while (re-search-forward "\r$" nil t)
		(replace-match "" t t))
	      (save-buffer)
	      (if (/= (file-modes buffer-file-name) ?\666)
		  (set-file-modes buffer-file-name ?\666))
	      (kill-buffer (current-buffer)))
	    (with-temp-buffer
	      (emacs-wiki-file-alist)	; force re-check
	      (insert "<redirect url=\"" page "\" delay=\"3\">")
	      (insert "Thank you, your changes have been saved to " page)
	      (insert ".  You will be redirected to "
		      "the new page in a moment.")
	      (insert "</redirect>")
	      (emacs-wiki-http-send-buffer "Changes Saved"))))))))

(defvar httpd-vars nil)

(defsubst httpd-var (var)
  "Return value of VAR as a URL variable.  If VAR doesn't exist, nil."
  (cdr (assoc var httpd-vars)))

(defsubst httpd-var-p (var)
  "Return non-nil if VAR was passed as a URL variable."
  (not (null (assoc var httpd-vars))))

(defun emacs-wiki-serve-page (page content)
  (let ((handled t))
    (cond
     ((string-match "\\`wiki\\?\\(.+\\)" page)
      (emacs-wiki-render-page (match-string 1 page)))

     ((string-match "\\`editwiki\\?\\(.+\\)" page)
      (let ((page-name (match-string 1 page)))
	(if (not (emacs-wiki-editable-p page-name))
	    (emacs-wiki-http-reject "Editing not allowed"
				    "Editing this Wiki page is not allowed")
	  (with-temp-buffer
	    (emacs-wiki-setup-edit-page page-name)
	    ;; this is required because of the : in the name
	    (emacs-wiki-http-send-buffer
	     (concat "Edit Wiki Page: " page-name))))))

     ((string-match "\\`searchwiki\\?get" page)
      (with-temp-buffer
	(insert "<verbatim>" emacs-wiki-http-search-form "</verbatim>")
	(emacs-wiki-http-send-buffer "Search Wiki Pages")))

     ((string-match "\\`searchwiki\\?q=\\(.+\\)" page)
      (let ((compilation-scroll-output nil)
	    (term (match-string 1 page)))
	(unintern 'start-process)
	(require 'compile)
	(with-current-buffer (emacs-wiki-grep term)
	  (emacs-wiki-wikify-search-results term)
	  (emacs-wiki-http-send-buffer "Search Results")
	  (kill-buffer (current-buffer)))))

     ((string-match "\\`changewiki\\?post" page)
      (emacs-wiki-http-changewiki content))

     ((string-match "\\`diffwiki\\?\\(.+\\)" page)
      ;; jww (2001-04-20): This code doesn't fully work yet.
      (emacs-wiki-find-file (match-string 1 page))
      (require 'vc)
      (require 'vc-hooks)
      (let ((curr-ver (vc-workfile-version buffer-file-name)))
	(vc-version-diff buffer-file-name
			 curr-ver (vc-previous-version curr-ver))
	(let ((inhibit-read-only t))
	  (goto-char (point-min))
	  (when (re-search-forward "^diff" nil t)
	    (forward-line)
	    (delete-region (point-min) (point)))
	  (insert "<verbatim><pre>")
	  (emacs-wiki-escape-html-specials)
	  (goto-char (point-max))
	  (if (re-search-backward "^Process.*killed" nil t)
	      (delete-region (point) (point-max)))
	  (insert "</verbatim></pre>")
	  (emacs-wiki-http-send-buffer "Diff Results"))))

     (t
      (setq handled nil)))
    handled))

(defun emacs-wiki-serve (page &optional content)
  "Serve the given PAGE from this emacs-wiki server."
  ;; index.html is really a reference to the main Wiki page
  (if (string= page "index.html")
      (setq page (concat "wiki?" emacs-wiki-home-page)))

  ;; handle the actual request
  (let ((vc-follow-symlinks t)
	(emacs-wiki-report-threshhold nil)
	(emacs-wiki-serving-p t)
	httpd-vars project)
    (save-excursion
      ;; process any CGI variables, if cgi.el is available
      (if (string-match "\\`\\([^&]+\\)&" page)
	  (setq httpd-vars
		(and (fboundp 'cgi-decode)
		     (cgi-decode (substring page (match-end 0))))
		page (match-string 1 page)))
      (setq project (httpd-var "project"))
      (if project
	  (with-emacs-wiki-project project
	    (emacs-wiki-serve-page page content))
	(emacs-wiki-serve-page page content)))))

(if (featurep 'httpd)
    (httpd-add-handler "\\`\\(index\\.html\\|.*wiki\\(\\?\\|\\'\\)\\)"
		       'emacs-wiki-serve))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Support for multile Emacs Wiki projects
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defgroup emacs-wiki-project nil
  "Options controlling multi-project behavior in Emacs-Wiki."
  :group 'emacs-wiki)

(defvar emacs-wiki-current-project nil)
(defvar emacs-wiki-predicate nil)
(defvar emacs-wiki-major-mode nil)
(defvar emacs-wiki-project-server-prefix nil)

(defcustom emacs-wiki-show-project-name-p t
  "When true, display the current project name in the mode-line"
  :group 'emacs-wiki
  :type 'boolean)

;; this might go away - did anyone prefer the old behavior? tell me!
(defvar emacs-wiki-old-project-change-p nil)

(defcustom emacs-wiki-update-project-hook
  '(emacs-wiki-update-project-interwikis)
  "A hook called whenever `emacs-wiki-projects' is modified.
By default, this hook is used to update the Interwiki table so that it
contains links to each project name."
  :type 'hook
  :group 'emacs-wiki-project)

(defun emacs-wiki-update-project-interwikis ()
  (let ((projs emacs-wiki-projects))
    (while projs
      (add-to-list
       'emacs-wiki-interwiki-names
       `(,(caar projs)
	 . (lambda (tag)
	     (emacs-wiki-project-interwiki-link ,(caar projs) tag))))
      (setq projs (cdr projs)))))

(defcustom emacs-wiki-projects nil
  "A list of project-specific Emacs-Wiki variable settings.
Each entry is a cons cell, of the form (PROJECT VARS).

Projects are useful for maintaining separate wikis that vary in
some way. For instance, you might want to keep your work-related
wiki files in a separate directory, with a different fill-column:

(setq emacs-wiki-projects
      `((\"default\" . ((emacs-wiki-directories . (\"~/wiki\"))))
        (\"work\" . ((fill-column . 65)
                 (emacs-wiki-directories . (\"~/workwiki/\"))))))

You can then change between them with \\[emacs-wiki-change-project],
by default bound to C-c C-v. When you use \\[emacs-wiki-find-file] to
find a new file, emacs-wiki will attempt to detect which project it
is part of by finding the first project where emacs-wiki-directories
contains that file.

VARS is an alist of symbol to value mappings, to be used locally in
all emacs-wiki buffers associated with that PROJECT.

You may also set the variable `emacs-wiki-predicate' in this alist,
which should be a function to determine whether or not the project
pertains to a certain buffer.  It will be called within the buffer in
question.  The default predicate checks whether the file exists within
`emacs-wiki-directories' for that project.

The variable `emacs-wiki-major-mode' can be used to determine the
major mode for a specific emacs-wiki buffer, in case you have
developed a customized major-mode derived from `emacs-wiki-mode'.

The variable `emacs-wiki-project-server-prefix' is prepended to the
Interwiki URL, whenever an Interwiki reference to another project is
made.  For example, if you had two projects, A and B, and in A you
made a reference to B by typing B#WikiPage, A needs to know what
directory or server to prepend to the WikiPage.html href.  If this
variable is not set, it is assumed that both A and B publish to the
same location.

If any variable is not customized specifically for a project, the
global value is used."
  :type `(repeat
	  (cons
	   :tag "Emacs-Wiki Project"
	   (string :tag "Project name")
	   (repeat
	    (choice
	     (cons :tag "emacs-wiki-predicate"
		   (const emacs-wiki-predicate) function)
	     (cons :tag "emacs-wiki-major-mode"
		   (const emacs-wiki-major-mode) function)
	     (cons :tag "emacs-wiki-project-server-prefix"
		   (const emacs-wiki-project-server-prefix) string)
	     ,@(mapcar
		(function
		 (lambda (sym)
		   (list 'cons :tag (symbol-name sym)
			 (list 'const sym)
			 (get sym 'custom-type))))
		(apropos-internal "\\`emacs-wiki-"
				  (function
				   (lambda (sym)
				     (and (not (eq sym 'emacs-wiki-projects))
					  (get sym 'custom-type))))))))))
  :set (function
	(lambda (sym val)
	  (set sym val)
	  (run-hooks 'emacs-wiki-update-project-hook)))
  :group 'emacs-wiki-project)

(defmacro with-emacs-wiki-project (project &rest body)
  "Evaluate as part of PROJECT the given BODY forms."
  `(with-temp-buffer
     (emacs-wiki-change-project ,project)
     ,@body))

(put 'with-emacs-wiki-project 'lisp-indent-function 1)

(defun emacs-wiki-change-project (project)
  "Manually change the project associated with the current buffer."
  (interactive (list (completing-read "Switch to project: "
				      emacs-wiki-projects
				      nil t nil)))
  (let ((projsyms (cdr (assoc project emacs-wiki-projects)))
	sym)
    (while projsyms
      (setq sym (caar projsyms))
      (unless (memq sym '(emacs-wiki-predicate emacs-wiki-major-mode))
	(let ((custom-set (or (get sym 'custom-set) 'set))
	      (var (if (eq (get sym 'custom-type) 'hook)
		       (make-local-hook sym)
		     (make-local-variable sym))))
	  (if custom-set
	      (funcall custom-set var (cdar projsyms)))))
      (setq projsyms (cdr projsyms))))
  (when (not (string= emacs-wiki-current-project project))
    ;; if it was a user request to change, change to the welcome buffer first
    (if (and (interactive-p)
               (not emacs-wiki-old-project-change-p))
        (with-emacs-wiki-project
         project (emacs-wiki-visit-link emacs-wiki-default-page))
      (set (make-local-variable 'emacs-wiki-current-project) project)
      (when emacs-wiki-show-project-name-p
        (setq mode-name (concat "Wiki[" project "]"))))))

(defun emacs-wiki-project-interwiki-link (project tag)
  (with-emacs-wiki-project project
    (if emacs-wiki-publishing-p
	(concat emacs-wiki-project-server-prefix
		(emacs-wiki-link-url tag))
      (or (emacs-wiki-page-file tag)
	  (expand-file-name tag (car emacs-wiki-directories))))))

(provide 'emacs-wiki)
;;; emacs-wiki.el ends here
