/* 
 * Ruby's lexer and parser, ast builder and json visitor originally written by Miquel Sabaté
 * and extended here for some new functionality.
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


#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "rubyastvisitor.h"
extern int rb_debug_file(struct options_t *opts); // функция для тестирования лексера и парсера из parser.c, который сгенерирован бизоном

/*
 * Комментарии к коду тоже хранятся :)
 */
void fetch_comments(struct Ruby::node *tree)
{
    if (!tree)
        return;
    if (tree->comment != NULL)
        printf("%s", tree->comment);

    fetch_comments(tree->l);
    fetch_comments(tree->r);
    fetch_comments(tree->next);
}



int main(int argc, char *argv[])
{
    struct Ruby::ast_t *ast; // само AST дерево
    struct Ruby::options_t opts; // входные параметры


    opts.path = argv[argc - 2]; // вход
	opts.to = argv[argc - 1]; // выход
    opts.contents = NULL;
    //opts.version = atoi(argv[argc - 1]);
	opts.version = Ruby::ruby21; // версия руби
	printf("Запускаем лексер и парсер\n");
    ast = Ruby::rb_compile_file(&opts); // получаем AST
	Ruby::RubyAstVisitor* visitor = new Ruby::RubyAstVisitor(ast->tree, &opts); // объект класса для обхода дерева и вывода результата в файл
	printf("Обходим дерево\n");
	visitor->visitCode(); // посещаем
	#ifdef BUILD_TESTS
		if (ast->errors) {
			Ruby::print_errors(ast->errors);
			exit(1);
		}
	#endif
    //fetch_comments(ast->tree);
    Ruby::rb_free(ast); // освобождаем дерево

    
    printf("Конец\n");
    
    return 0;
}
