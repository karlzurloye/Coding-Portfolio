
(defun longest-path (start end net)
   (or (reverse (dfs (list start) end net))
       (when (eql start end) (list start))))


(defun longer (x-new x-old)
  (if (> (length x-new) (length x-old)) x-new
    x-old))


(defun dfs (start end net)
  (do ((pathstarts (cdr (assoc (car start) net)) (cdr pathstarts))
       (maxlist '() (longer maxlist (pathfinder start (car pathstarts) end net))))
      ((null pathstarts) maxlist)))


(defun pathfinder (lst node end net)
  (cond ((eql node end) (cons end lst))
        ((find node lst) nil)
        (t (dfs (cons node lst) end net))))






