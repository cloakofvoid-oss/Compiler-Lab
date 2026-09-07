# C Program to Check the First and Follow
## Date :
## Algorithm
    Start
    Calculating first, α → t β
    if α is a terminal, then FIRST(α) = { α }.
    if α is a non-terminal and α → ℇ is a production, then FIRST(α) = { ℇ }.
    if α is a non-terminal and α → 𝜸1 𝜸2 𝜸3 … 𝜸n and any FIRST(𝜸) contains then t is in FIRST(α).
    Calculating follow,
    if α is a start symbol, then FOLLOW() = $
    if α is a non-terminal and has a production α → AB, then FIRST(B) is inFOLLOW(A) except ℇ.
    if α is a non-terminal and has a production α → AB, where B ℇ, thenFOLLOW(A) is in FOLLOW(α).
    Stop

## Output : 
