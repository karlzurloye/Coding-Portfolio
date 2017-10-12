
; part a

(defun shortest-path (start end net)
  (catch 'path_found
    (bfs end (list (list start)) net)))

(defun bfs (end queue net)
  (if (empty-queue-p queue) nil
    (let ((path (car queue)))
      (let ((node (car path)))
        (cond ((eql node end) (reverse path))
              ((loop-test path) nil)
              (t (bfs end
               (append (cdr queue)
                       (new-paths path node net end))
               net)))))))

(defun new-paths (path node net end)
  (mapcar #'(lambda (n)
              (when (eql n end) 
                  (throw 'path_found (reverse (cons n path))))
              (cons n path))
          (cdr (assoc node net))))

(defun loop-test (path)
  (cond ((null (cdr path)) nil)
        ((eql (car path) (cadr path)) t)
        (t (loop-test (cons (car path) (cddr path))))))

; part b

(defun shortest-path (start end net)
    (bfs end (list (list start)) net))

(defun bfs (end queue net)
  (if (empty-queue-p queue) nil
    (let ((path (car queue)))
      (let ((node (car path)))
        (cond ((loop-test path) nil)
              ((find end (cdr (assoc node net)))
               (reverse (cons end path)))
              (t (bfs end (append (cdr queue) (new-paths path node net end))
                      net)))))))

(defun new-paths (path node net end)
  (mapcar #'(lambda (n)
              (cons n path))
          (cdr (assoc node net))))

(defun loop-test (path)
  (cond ((null (cdr path)) nil)
        ((eql (car path) (cadr path)) t)
        (t (loop-test (cons (car path) (cddr path))))))

