/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlo                                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:38:26 by carlo             #+#    #+#             */
/*   Updated: 2024/11/03 19:55:52 by carlo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_pd.h"

#include <stdlib.h>

static t_class *except_class;

typedef struct _except {
  t_object      exObj;
 
  t_float       input_number;

  t_float       *ignore;
  t_int         ignore_size;

  t_outlet      *filtered, *exceptions;

}               t_except;

/**
 * @brief Check if the input number is in the ignore list
 *      and send it to the left or right outlet
 * 
 * @param x the except object instance
 * @param f the input number
*/

void check_number(t_except *x, t_floatarg f) {
  x->input_number = f;

  if (x->ignore_size == -1) {
    pd_error(x, "except: No ignore list provided");
    outlet_float(x->filtered, x->input_number);
    return;
  }

  t_int found = 0;
  for (int i = 0; i < x->ignore_size; i++) {
    if (x->input_number == x->ignore[i]) {
      found = 1;
      return;
    }
    found = 0;
  }
  found ? outlet_float(x->exceptions, x->input_number) : outlet_float(x->filtered, x->input_number);
}

/**
 * @brief Get the ignored numbers from the input list
*/
t_float* get_ignored(t_except *x, t_symbol *s, int argc, t_atom *argv) {
  (void)s;
  x->ignore_size = argc;
  x->ignore = (t_float *)calloc(x->ignore_size, sizeof(t_float));

  if(!x->ignore) {
    pd_error(x, "Except object: Memory allocation failed");
    return NULL;
  }

  for (int i = 0; i < x->ignore_size; i++) {
    x->ignore[i] = atom_getfloat(argv + i);
  }

  return x->ignore;
}

/**
 * @brief Free the memory allocated for the ignore list
*/
void except_free(t_except *x) {
  if (x->ignore_size != -1 && x->ignore != NULL) {
    free(x->ignore);
  }
  outlet_free(x->filtered);
  outlet_free(x->exceptions);
}

/**
 * @brief Create a new except object
 *          with the ignore list if provided
*/
void *except_new(t_symbol *s, int ac, t_atom *av) {
  t_except *x = (t_except *)pd_new(except_class);
 
  (void)s;

  if (ac > 0) {
    x->ignore_size = ac;
    x->ignore = (t_float *)calloc(ac, sizeof(t_float));   

    if (!x->ignore) {
      pd_error(x, "Except object: Memory allocation failed");
      return NULL;
    }

    for (int i = 0; i < ac; i++) {
      x->ignore[i] = atom_getfloat(av+i);
    }
  } else {
    x->ignore_size = -1;
  }

  inlet_new(&x->exObj, &x->exObj.ob_pd, &s_list, gensym("get_ignored"));

  x->filtered = outlet_new(&x->exObj, &s_float);
  x->exceptions = outlet_new(&x->exObj, &s_float);

  return (void *)x;
}

/**
 * @brief Setup the except class
*/
void except_setup(void) {
  except_class = class_new(gensym("except"), (t_newmethod)except_new, (t_method)except_free, 
                           sizeof(t_except), CLASS_DEFAULT, A_GIMME, 0); 

  class_addfloat(except_class, check_number);
  class_addmethod(except_class, (t_method)get_ignored, gensym("get_ignored"), A_GIMME, 0);

  class_sethelpsymbol(except_class, gensym("except"));
}
