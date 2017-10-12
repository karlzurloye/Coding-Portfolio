

(defstruct 3tree
  data
  left
  middle
  right)
  

(defun 3tree-clone (tree0)
  (if (null tree0) nil
    (make-3tree :data (3tree-data tree0)
                :left (3tree-clone (3tree-left tree0))
                :middle (3tree-clone (3tree-middle tree0))
                :right (3tree-clone (3tree-right tree0)))))
                   

(defun 3tree-member (x tree)
  (cond ((null tree) nil)
        ((eql x (3tree-data tree)) t)
        ((3tree-member x (3tree-left tree)) t)
        ((3tree-member x (3tree-middle tree)) t)
        ((3tree-member x (3tree-right tree)) t)
        (t nil)))












