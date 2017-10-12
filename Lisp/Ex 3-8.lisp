
(defun show-dots (lst)
  (cond ((atom lst) (format t "~A" lst))
        (t (format t "(")
           (show-dots (car lst))
           (format t " . ")
           (show-dots (cdr lst))
           (format t ")"))))
        

(defun show-list (lst)
  (cond ((atom lst) (format t "~A" lst))
        (t (format t "[")
           (do ((x lst (cdr x)))
               ((atom (cdr x)) 
                (show-list (car x))
                (unless (null (cdr x)) (format t " . ~A" (cdr x))))
             (show-list (car x))
             (format t " "))
           (format t "]"))))



