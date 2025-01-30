/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:31:03 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/30 14:31:04 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void init_minishell(t_shell *shell, char **envp)
{
    if (!isatty(STDIN_FILENO))
    {
        // If not a TTY, print an error and exit
        fprintf(stderr, "Error: Minishell must be run in a terminal (TTY)\n");
        exit(1);
    }
    shell->env_list = init_envp(envp);
    if (!shell->env_list)
    {
        printf("Error: env list is NULL\n");
        exit(1);
    }
    shell->envp = envp_to_str(shell->env_list);
    if (!shell->envp || !shell->envp[0])
    {
        printf("Error: envp_to_str returned NULL or empty\n");
        free_envp_list(shell->env_list);
        exit(1);
    }
    shell->exit_status = 0;
}

void minishell_loop(t_shell *shell)
{
    char *line;
    t_token *tokens;
    t_ast *ast;

    while (1)
    {
        line = readline("minishell $>");
        if (!line)
            break;
        if (ft_strlen(line) > 0)
        {
            add_history(line);
            tokens = lexer(line);
            parse_tokens(tokens, &ast, shell->env_list);
            print_ast(ast);
            // Execute or traverse the AST here
            free_parsed_data(ast);
            // Free token list if needed
        }
        if (ft_strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }
        free(line);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    t_shell shell;

    init_minishell(&shell, envp);
    signals_t3res();
    minishell_loop(&shell);
    free_envp_list(shell.env_list);
    free_envp_array(shell.envp);
    return (shell.exit_status);
}