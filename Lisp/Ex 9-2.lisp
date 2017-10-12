

(defun make-change (goal &optional (coins '(25 10 5 1)))
  (coin-counter goal coins nil))
          

(defun coin-counter (goal coins values)
  (if (null coins) (values-list (reverse values))
    (multiple-value-bind (count remaining)
        (floor goal (car coins))
      (coin-counter remaining (cdr coins) (cons count values)))))



