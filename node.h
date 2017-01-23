/* 
 * Copyright (C) 2014 Sergei Ovchinnikov <rubasketcom@gmail.com>
 * Copyright (C) 2010 Miquel Sabaté <mikisabate@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef NODE_H_
#define NODE_H_


#ifdef __cplusplus // для совместимости

namespace Ruby {
extern "C" {
#endif

/**
 * Все возможные токены, используемые парсером/типы вершин в дереве.
 */
enum node_t {
    token_invalid = 0,
    token_comment = 1,      /* Not used anymore */
    token_plus,
    token_minus,
    token_mul,
    token_pow,
    token_div,
    token_mod,
    token_bit_and,
    token_bit_or,
    token_bit_xor,          /* 10 */
    token_kw_and,
    token_kw_or,
    token_kw_not,
    token_or,
    token_and,
    token_lshift,
    token_rshift,
    token_neg,
    token_not,
    token_unary_plus,       /* 20 */
    token_unary_minus,
    token_assign,
    token_op_assign,
    token_assoc,
    token_cmp,
    token_eq,
    token_neq,
    token_eqq,
    token_match,
    token_nmatch,           /* 30 */
    token_greater,
    token_geq,
    token_lesser,
    token_leq,
    token_dot2,
    token_dot3,
    token_ternary,
    token_if,
    token_unless,
    token_while,            /* 40 */
    token_until,
    token_case,
    token_when,
    token_up_begin,
    token_up_end,
    token_for,
    token_begin,
    token_rescue_arg,
    token_rescue,
    token_ensure,           /* 50 */
    token_object,
    token_numeric,
    token_symbol,
    token_body,
    token_function,
    token_module,
    token_class,
    token_singleton_class,
    token_super,
    token_string,           /* 60 */
    token_regexp,
    token_key,
    token_array,
    token_hash,
    token_block,
    token_method_call,
    token_heredoc,
    token_break,
    token_redo,
    token_retry,            /* 70 */
    token_next,
    token_return,
    token_yield,
    token_alias,
    token_defined,
    token_undef,
    token_array_value,
    token__end__,
    token_true,
    token_false,            /* 80 */
    token_nil,
    token_self,
    token_encoding,
    token_file,
    token_line,             /* 85 */
	token_bare_string,
};


/**
 * Хранит ошибки в ходе парсинга.
 */
struct error_t {
    char *msg;
    int line, column;
    unsigned char warning : 1;
    struct error_t *next;
};

/**
 * Это дерево, сгенерированное парсером. Содержит ошибки, а так же полезную статистику
 */
struct ast_t {
    struct node *tree;
    struct error_t *errors;
    unsigned char unrecoverable : 1;
};

/**
 * Позиция внутри самого документа, который парсили
 */
struct pos_t {
    int start_line, end_line;
    int start_col, end_col;
    unsigned long long offset;
};

/**
 * Задаем конкретные типы переменных.
 */
enum flags_t {
    /* Numeric literals. */
    int_l = 0,
    float_l,
    rational_l,
    imaginary_l,

    /* Variables */
    var,
    ivar,
    cvar,
    constant,
    global,

    /* Args */
    kwrest, // An argument that is: *arg. Example: def foo(*a); end
    star, // An argument that is just the star. Example: def foo(*); end
    label, // An argument with the form of arg: value. Example: def foo(a: 0); end
    opt, // An argument with the form of a = 0. Example: def foo(a = 0); end
    block, // An argument describing a block. Example: def foo(&a); end
};

/**
 * Узел в абстрактном синтаксическом дереве
 */
struct node {
/* Инфо о узле */
    int kind; //тип узла
    enum flags_t flags;  //флаги выше
    char *name;  //имена переменных
    void *context;
	char *value;
	
/* Node's position */
    struct pos_t pos;
    char *comment;

/* childs left/right:  - l -> например, для if содержит внутренние стейтменты.
   - r -> стейтменты else.*/
    struct node *l;
    struct node *r;

/* Expression условия : например, для while cond содержит условие. Для стейтмента класса  cond используется, чтобы содержать суперкласс и т.д.*/
    struct node *cond;

/* Ensure expression (Exception-AST only) */
    struct node *ensure;

/* next/last используются, например, для списка стейтментов */
    struct node *next;
    struct node *last;
};

/*
 * для удобства ;)
 */
typedef struct node Node;

/**
 * Версия руби для анализа
 */
enum ruby_version {
    ruby18 = 0, /* 1.8.x branch */
    ruby19 = 1, /* 1.9.x branch */
    ruby20 = 2, /* 2.0.x branch */
    ruby21 = 3, /* 2.1.x branch */
};

/**
 * Входные опции парсера.
 */
struct options_t {
    const char *path;
	const char *to;
    char *contents;
    enum ruby_version version;
};

/* Интерфейс парсера */

/**
 * Генерирует AST по входному файлу ruby.
 *
 * @param opts опции, предоставленные парсеру.
 * @return AST дерево, представляющее код.
 */
struct ast_t * rb_compile_file(struct options_t *opts);

/**
 * Очищаем ast_t.
 *
 * @param ra дерево ast_t , которое надо освободить.
 */
void rb_free(struct ast_t *ra);

/**
 * Get the name node.
 *
 * @param n The root node.
 */
struct node * rb_name_node(struct node *n);

/**
 * Очищаем узел ast_t. Уде вызывается функцией rb_free .
 *
 * @param n корень дерева.
 */
void free_ast(struct node *n);

/**
 * Очищаем ошибки ast_t. Уже вызывается rb_free.
 *
 * @param ra  ast_t содержит ошибки для удаления.
 */
void free_errors(struct ast_t *ra);

/*
	 * Есть 3 способа расположить узел. Первый самый простой - 
	 * alloc_node. Если необходимо поле conditional expression - alloc_cond. Если нужно поле исключение - alloc_ensure.
 */
 
struct node * alloc_node(int kind, struct node *l, struct node *r);
struct node * alloc_cond(int kind, struct node *cond, struct node *l,
                         struct node *r);
struct node * alloc_ensure(int kind, struct node * l, struct node *r,
                           struct node *els, struct node *ensure);


/*
 * Эти функции для того, чтобы играть с пред/след элементами узла.
 * Создание/добавление/конкатенация,
 */
struct node * create_list(struct node *head, struct node *tail);
struct node * update_list(struct node *head, struct node *tail);
struct node * concat_list(struct node *head, struct node *tail);
#define pop_list(head, tail) \
    (tail->last == NULL) ? update_list(head, tail) : create_list(head, tail)


/* Дебаг */

#ifdef BUILD_TESTS
void print_node(struct node *n);
void print_errors(struct error_t *errors);
#endif


/*
 * Дополнительные макросы.
 */

#define get_last_expr(n) ((n->last) ? n->last : n)
#define is_valid(n) (n->pos.start_line >= 0)
#define is_rest_arg(n) (n->flags == kwrest)
#define is_block_arg(n) (n->flags == block)
#define is_global_var(n) (n->flags == global)
#define is_ivar(n) (n->flags == ivar)
#define is_cvar(n) (n->flags == cvar)
#define is_constant (n->flags == constant)
#define valid_children(n) (n->r && n->l)
#define is_super(n) (!n->l)
#define has_star(n) (n->flags == kwrest || n->flags == star)
#define is_just_a_star(n) (n->flags == star)


#ifdef __cplusplus
}
}
#endif


#endif /* NODE_H_ */
