

(defun file-subst (old new file1 file2)
  (with-open-file (in file1 :direction :input)
     (with-open-file (out file2 :direction :output
                                :if-exists :supersede)
       (stream-subst old new in out))))


(defun stream-subst (old new in out &key (wildcard))
  (let* ((pos 0)
         (len (length old))
         (buf (new-buf len))
         (from-buf nil))
    (do ((c (read-char in nil :eof)
            (or (setf from-buf (buf-next buf))
                (read-char in nil :eof))))
        ((eql c :eof))
      (cond ((element-match (elt old pos) wildcard c)
             (incf pos)
             (cond ((= pos len)            ; 3
                    (princ new out)
                    (setf pos 0)
                    (buf-clear buf))
                   ((not from-buf)         ; 2
                    (buf-insert c buf))))
            ((zerop pos)                   ; 1
             (princ c out)
             (when from-buf
               (buf-pop buf)
               (buf-reset buf)))
            (t                             ; 4
             (unless from-buf
               (buf-insert c buf))
             (princ (buf-pop buf) out)
             (buf-reset buf)
             (setf pos 0))))
    (buf-flush buf out)))


(defun element-match (pattern wildcard c)
  (if (characterp pattern)
      (or (char= c pattern)
          (char= wildcard pattern))
    (ecase pattern
      (:digit (digit-char-p c))
      (:alpha (alpha-char-p c))
      (:alphanumeric (alphanumericp c))
      (:wild t))))
  

 
;;; backup

(cond ((element-match old pos wildcard c)

(defun element-match (old pos wildcard c)
  (if (stringp old)
      (let ((old-char (char old pos)))
        (or (char= c old-char)
            (char= wildcard old-char)))
    (ecase (svref old pos)
      (:digit (digit-char-p c))
      (:alpha (alpha-char-p c))
      (:alphanumeric (alphanumericp c))
      (:wild t))))

(cond ((if (stringp (subseq old pos (1+ pos)))
                 (or (char= c (char old pos))
                     (char= wildcard (char old pos)))
               (or (and (equalp #(:digit) (subseq old pos (1+ pos))) (digit-char-p c))
                   (and (equalp #(:alpha) (subseq old pos (1+ pos))) (alpha-char-p c))
                   (and (equalp #(:alphanumeric) (subseq old pos (1+ pos))) (alphanumericp c))
                   (equalp #(:wild) (subseq old pos (1+ pos)))))

;;; book code

(defstruct buf
  vec (start -1) (used -1) (new -1) (end -1))

(defun bref (buf n)
  (svref (buf-vec buf)
         (mod n (length (buf-vec buf)))))

(defun (setf bref) (val buf n)
  (setf (svref (buf-vec buf)
               (mod n (length (buf-vec buf))))
        val))

(defun new-buf (len)
  (make-buf :vec (make-array len)))

(defun buf-insert (x b)
  (setf (bref b (incf (buf-end b))) x))

(defun buf-pop (b)
  (prog1 
    (bref b (incf (buf-start b)))
    (setf (buf-used b) (buf-start b)
          (buf-new  b) (buf-end   b))))

(defun buf-next (b)
  (when (< (buf-used b) (buf-new b))
    (bref b (incf (buf-used b)))))

(defun buf-reset (b)
  (setf (buf-used b) (buf-start b)
        (buf-new  b) (buf-end   b)))

(defun buf-clear (b)
  (setf (buf-start b) -1 (buf-used  b) -1
        (buf-new   b) -1 (buf-end   b) -1))

(defun buf-flush (b str)
  (do ((i (1+ (buf-start b)) (1+ i)))
      ((> i (buf-end b)))
    (princ (bref b i) str)))