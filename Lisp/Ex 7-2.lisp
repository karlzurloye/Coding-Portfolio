
(defun map-file (function pathname)
  (with-open-file (stream file :direction :input)
    (map-stream function stream)))


(defun map-stream (function stream)
  (let ((end (cons 'a 'b)))
    (do ((expr (read stream nil end) (read stream nil end))
         (out '() (cons (funcall function expr) out)))
        ((eql expr end) out))))
            




