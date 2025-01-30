#include "../include/minishell.h"

// Function to create a new AST node
t_ast *create_ast_node(t_node_type type, char **args, char *file)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->type = type;
    node->args = args;
    node->file = file;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to build the AST from tokens
t_ast *build_ast(t_token *tokens)
{
    t_ast *root = NULL;
    t_ast *current = NULL;
    t_token *token = tokens;

    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            char **args = malloc(sizeof(char *) * 2);
            args[0] = ft_strdup(token->value);
            args[1] = NULL;
            t_ast *cmd_node = create_ast_node(NODE_COMMAND, args, NULL);
            if (!root)
                root = cmd_node;
            else if (current)
                current->right = cmd_node;
            current = cmd_node;
        }
        else if (token->type == TOKEN_PIPE)
        {
            t_ast *pipe_node = create_ast_node(NODE_PIPE, NULL, NULL);
            pipe_node->left = root;
            root = pipe_node;
            current = root;
        }
        else if (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_REDIRECT_OUT ||
                 token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
        {
            char *file = ft_strdup(token->next->value);
            t_ast *redir_node = create_ast_node(NODE_REDIR_IN, NULL, file);
            redir_node->left = root;
            root = redir_node;
            current = root;
            token = token->next; // Skip the next token as it is the file name
        }
        token = token->next;
    }

    return root;
}

// Syntax error checking function
int check_syntax_errors(t_token *tokens)
{
    t_token *curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_PIPE && (!curr->next || curr->next->type == TOKEN_PIPE))
            return (printf("Syntax error: unexpected token `|`\n"), 1);
        if ((curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_REDIRECT_OUT ||
             curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC) &&
            (!curr->next || curr->next->type != TOKEN_WORD))
            return (printf("Syntax error: unexpected token `%s`\n", curr->value), 1);
        curr = curr->next;
    }
    return 0;
}

// Expanding environment variables
char *expand_variables(char *input, t_env *env_list)
{
    char *expanded = NULL;
    char *var;
    while ((var = ft_strchr(input, '$')))
    {
        char *end = var + 1;
        while (*end && (ft_isalnum(*end) || *end == '_'))
            end++;
        char *var_name = ft_substr(var + 1, 0, end - (var + 1));
        char *var_value = ft_getenv(var_name, env_list);
        expanded = ft_strjoin(expanded, ft_substr(input, 0, var - input));
        expanded = ft_strjoin(expanded, var_value);
        input = end;
        free(var_name);
    }
    return expanded ? expanded : ft_strdup(input);
}

// Handling here-document (<<)
void handle_heredoc(char *delimiter)
{
    char *line;
    int p[2];
    pipe(p);
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
            break;
        write(p[1], line, ft_strlen(line));
        write(p[1], "\n", 1);
        free(line);
    }
    close(p[1]);
    free(line);
}

// Improved parser function
void parse_tokens(t_token *tokens, t_ast **ast, t_env *env_list)
{
    if (check_syntax_errors(tokens))
        return;
    while (tokens)
    {
        if (tokens->type == TOKEN_HEREDOC)
            handle_heredoc(tokens->next->value);
        else if (tokens->type == TOKEN_WORD)
            tokens->value = expand_variables(tokens->value, env_list);
        tokens = tokens->next;
    }
    *ast = build_ast(tokens);
}

// Freeing memory after parsing
void free_parsed_data(t_ast *ast)
{
    if (!ast)
        return;
    free_parsed_data(ast->left);
    free_parsed_data(ast->right);
    free(ast);
}

void print_ast(t_ast *ast)
{
    if (!ast)
        return;
    if (ast->args)
    {
        int i = 0;
        while (ast->args[i])
            printf("args[%d]: %s\n", i, ast->args[i]), i++;
    }
    if (ast->file)
        printf("file: %s\n", ast->file);
    print_ast(ast->left);
    print_ast(ast->right);
}