(defun greater (a b)
  (if (> a b) a
    b))
  


(defun has-list-p (lst)
  (cond ((null lst) nil)
        ((listp (car lst)) t)
        (t (has-list-p (cdr lst)))))



(defun print-dots (num)
  (do ((ii 1 (1+ ii)))
      ((> ii num) t)
    (format t ".")))

(defun print-dots (num)
  (cond ((= 0 num) t)
        (t (format t ".")
         (print-dots (1- num)))))



(defun get-a-count (lst)
  (do ((x lst (cdr x))
       (y 0 (if (eql (car x) 'a)
                (1+ y)
              y)))
      ((null x) y)))

(defun get-a-count (lst)
  (cond ((null lst) 0)
        ((eql (car lst) 'a) (1+ (get-a-count (cdr lst))))
        (t (get-a-count (cdr lst)))))



(defun summit (lst)
  (apply #'+ (remove nil lst)))

(defun summit (lst)
  (if (null lst) 0
    (let ((x (car lst)))
      (if (null x)
          (summit (cdr lst))
        (+ x (summit (cdr lst)))))))

;;; a) (remove nil lst) returns lst with nils removed, but does not alter lst. Thus (apply #'+ lst) is using lst with nils, not the output of (remove nil lst)
;;; b) Since the 'if' statement will result in another call of the function regardless of whether 'if' is true or not, without (if (null lst) 0, there is no way to exit the loop. 



    