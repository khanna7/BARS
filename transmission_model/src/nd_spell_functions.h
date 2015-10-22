/*
 *  File networkDynamic/src/spellfunctions.h
 *  Part of the statnet package, http://statnetproject.org
 *
 *  This software is distributed under the GPL-3 license.  It is free,
 *  open source, and has the attribution requirements (GPL Section 7) in
 *    http://statnetproject.org/attribution
 *
 *  Copyright 2012 the statnet development team
 */

#ifndef SRC_ND_SPELL_FUNCTIONS_H_
#define SRC_ND_SPELL_FUNCTIONS_H_

#include <R.h>
#include <Rinternals.h>


SEXP InsertSpell(SEXP spell_list, double onset, double terminus, Rboolean debug_output);

#endif /* SRC_ND_SPELL_FUNCTIONS_H_ */
