; 6. <literal> -> NUMLIT_T
5
; 7. <literal> -> SYMBOL_T

; 8. <literal> -> QUOTE_T <quoted_lit>
; 9. <quoted_lit> -> SYMBOL_T
; 10. <quoted_lit> -> NUMLIT_T
; 11. <quoted_lit> -> LPAREN_T <stmt_list> RPAREN_T
'a
'aa
'(a b)
'(a 1 2 a)
; 12. <action> -> DEFINE_T LPAREN_T SYMBOL_T <param_list> RPAREN_T <stmt>
; 13. <param_list> -> SYMBOL_T <param_list> -- tested

; 14. <param_list> -> <>  -- tested
; 15. <action> -> IF_T <stmt> <stmt> <else_part>
; 16. <else_part> -> <stmt> -- tested
; 17. <else_part> -> <> -- tested



(define (b)(if (> 1 2) 1 2))
(define (aa c) c)
(define (ab ) 1)
(define (ac ) '1)

; 18. <action> -> CAR_T <stmt> -- tested
(car 'a)
; 19. <action> -> CDR_T <stmt> -- tested
(cdr 'a)
; 20. <action> -> CONS_T <stmt> <stmt> 
(cons 1 'a)
; 21. <action> -> AND_T <stmt_list>

(and 1 2)
; 22. <action> -> OR_T <stmt_list>

(or 1 2)
; 23. <action> -> NOT_T <stmt>
(not 1)
(not (not 1))
; 24. <action> -> LISTOP_T <stmt> -- tested
; 25. <action> -> NUMBERP_T <stmt>
(number? 1)
(number? (number? 1))
; 26. <action> -> SYMBOLP_T <stmt>
(symbol? 1)
(symbol? (symbol? 1))
; 27. <action> -> LISTP_T <stmt>
(list? 1)
(list? (list? 1))
; 28. <action> -> ZEROP_T <stmt>
(zero? 1)
(zero? (zero? 1))
; 29. <action> -> NULLP_T <stmt>
(null? 1)
(null? (null? 1))
; 30. <action> -> CHARP_T <stmt>
(char? 1)
(char? (char? 1))
; 31. <action> -> STRINGP_T <stmt>
(string? 1)
(string? (string? 1))
; 32. <action> -> PLUS_T <stmt_list>
(+ 1 2)
; 33. <action> -> MINUS_T <stmt> <stmt_list>
(- 1 2)
; 34. <action> -> DIV_T <stmt> <stmt_list>
(/ 1 2)
; 35. <action> -> MULT_T <stmt_list>

(* 1 2)
; 36. <action> -> EQUALTO_T <stmt_list> 


(= 1 2)
; 37. <action> -> GT_T <stmt_list>

(> 1 2)
; 38. <action> -> LT_T <stmt_list>


(< 1 2)
; 39. <action> -> GTE_T <stmt_list>


(>= 1 2)
; 40. <action> -> LTE_T <stmt_list>


(<= 1 2)
; 41. <action> -> SYMBOL_T <stmt_list>
(a 1)
(a 1 2)

