/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:22:52 by abdsalah          #+#    #+#             */
/*   Updated: 2025/02/01 23:38:27 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void init_minishell(t_shell *shell, char **envp)//$SHLVL
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

	while (1)
	{
		line = readline("minishell $>");
		if (!line)
			break;
		if (ft_strlen(line) > 0)
			add_history(line);
		if (ft_strcmp(line, "exit") == 0)//temp 
		{
			free(line);
			break;
		}if (ft_strncmp(line, "cd",2) == 0)//temp 
		{
			ft_cd(line+2, &shell->env_list);
		}
		if (ft_strcmp(line, "env") == 0)//temp 
		{
			ft_env(shell);
			// free(line);
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
