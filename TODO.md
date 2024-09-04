Rq: on aimerait bien être capables de
  * gérer les lignes de commande... => ça se fera sans doute avec un scope particulier (possibilité de réassigner, etc...)
  * fontionner avec du streaming => pourquoi pas, mais ça se fera en fonction des besoins

Proposition: une machine à état minimal pour parser
  start
    \n        => lfeed
    12 13 ... => ...

  start_lfeed: curr_tok.clear(); curr_tok += c;
    space     => cont_lfeed
    ...       => start

  cont_lfeed: curr_tok += c;
    space     => cont_lfeed
    ...       => start
    
On pourrait faire un remplissage "simple" avec des caractères sur un octet. 

Prop: 
  lin: LF SP*
  ope: OP OP* (  )?
  num: NU AL*
  lin: LF SP*
  
  
Pb: il faudrait que Displayer 