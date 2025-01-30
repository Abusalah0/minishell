/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdsalah <abdsalah@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 03:09:26 by abdsalah          #+#    #+#             */
/*   Updated: 2025/01/30 13:52:23 by abdsalah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *create_token(t_token_type type, char *value)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->value = ft_strdup(value);
    new->next = NULL;
    return (new);
}

void add_token(t_token **head, t_token_type type, char *value)
{
    t_token *new = create_token(type, value);
    if (!new)
        return;
    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

t_token *lexer(char *input)
{
    int i = 0;
    t_token *tokens = NULL;
    while (input[i])
    {
        if (input[i] == ' ' || input[i] == '\t') // Skip spaces
            i++;
        else if (input[i] == '|')
            add_token(&tokens, TOKEN_PIPE, "|"), i++;
        else if (input[i] == '<' && input[i + 1] == '<')
            add_token(&tokens, TOKEN_HEREDOC, "<<"), i += 2;
        else if (input[i] == '>' && input[i + 1] == '>')
            add_token(&tokens, TOKEN_APPEND, ">>"), i += 2;
        else if (input[i] == '<')
            add_token(&tokens, TOKEN_REDIRECT_IN, "<"), i++;
        else if (input[i] == '>')
            add_token(&tokens, TOKEN_REDIRECT_OUT, ">"), i++;
        else if (input[i] == '\'' || input[i] == '"') // Handle quoted strings
        {
            char quote = input[i++];
            int start = i;
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
                i++; // Move past closing quote
            add_token(&tokens, TOKEN_WORD, ft_substr(input, start - 1, i - start + 1));
        }
        else // Word (command or argument)
        {
            int start = i;
            while (input[i] && input[i] != ' ' && input[i] != '|' &&
                   input[i] != '<' && input[i] != '>')
            {
                if (input[i] == '\'' || input[i] == '"') // Merge quotes inside word
                {
                    char quote = input[i++];
                    while (input[i] && input[i] != quote)
                        i++;
                    if (input[i] == quote)
                        i++;
                }
                else
                    i++;
            }
            add_token(&tokens, TOKEN_WORD, ft_substr(input, start, i - start));
        }
    }
    return (tokens);
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: %s\n", tokens->value);
        tokens = tokens->next;
    }
}
