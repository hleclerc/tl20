La proposition pour les chaines de caractère, c'est de mettre les noms de variables dans un tableau global
Pour les String, on les aura en inline.

Par contre, pour les url des sources, ça n'a pas de sens de le mettre en global. La proposition, ça pourrait être de faire AString, qui pointe sur un Ast particulier. Rq: on va devoir pointer sur un Ast en construction ou un Ast déjà construit.

L'approche pourrait être d'avoir dans l'Ast une partie en construction après une partie déjà construite, et on stocke des offsets.
    -> ça sera très bien pour les strings. Pour les node par contre, il faudrait tolérer d'avoir une version non utilisable tant que ce n'est pas compacté.

Si on stocke des offsets, il faudrait un moyen d'accélérer la recherche lorsque les données sont fractionnées. 
    Ou alors, on se débrouille pour que les données soient contigues.
