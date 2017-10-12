

(defun max-min (v &key (start 0) (end (length v)))
  (if (= start end) (values nil nil)
    (max-min-rec v (1+ start) end (svref v start) (svref v start))))

(defun max-min-rec (v index end min max)
  (if (= index end) (values max min)
    (let ((x (svref v index)))
      (max-min-rec v (1+ index) end 
                   (min x min)
                   (max x max)))))


;;; backup

(defun max-min (v &key ((:start start) 0) ((:end end) (length v)))
  (if (= start end) (values nil nil)
    (max-min-rec v (1+ start) end (svref v start) (svref v start))))

(defun max-min-rec (v index end min max)
  (if (= index end) (values max min)
    (let ((x (svref v index)))
      (max-min-rec v (1+ index) end 
                   (if (< x min) x min)
                   (if (> x max) x max)))))


;;;

(defun extreme (v)
  (extreme-rec v 1 (length v) (svref v 0) (svref v 0)))

(defun extreme-rec (v i n mn mx)
  (if (= i n) (values mn mx)
    (let ((x (svref v i)))
      (extreme-rec v (+ i 1) n (if (< x mn) x mn) (if (< mx x) x mx)))))

;;;

(defun max-min (vec &key (start 0) (end (length vec)) min max)
  (cond ((or (= end 0) (= start end)) (values max min))
        (t (let ((curr (svref vec start)))
             (max-min vec :max (greater curr  max) :min (lesser curr min) :start (1+ start) :end end)))))

(defun greater (x y)
  (cond ((or (null x) (< x y)) y)
        (t x)))

(defun lesser (x y)
  (cond ((or (null x) (< y x)) y)
        (t x)))
