

(defun solve (f min max eps)
  (let ((max-out (funcall f max))
        (min-out (funcall f min)))
    (cond ((< 0 (* min-out max-out))
           (error "no solution"))
          ((= 0 min-out) min)
          ((= 0 max-out) max)
          ((< min-out max-out)
           (num-solve f min max eps))
          (t (num-solve f max min eps)))))


(defun num-solve (f m1 m2 eps)
  (let ((midpt (* 0.5 (+ m1 m2))))
    (if (< (abs (- m1 m2)) eps) midpt
      (let ((midpt-out (funcall f midpt)))
        (cond ((= 0 midpt-out) midpt)
              ((< 0 midpt-out) (num-solve f m1 midpt eps))
              (t (num-solve f midpt m2 eps)))))))

