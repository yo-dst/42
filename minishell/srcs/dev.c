#include "minishell.h"

void	print_tokens(t_list *tokens)
{
	t_token	*tok;

	ft_printf("\033[1;33mTOKENS\n\033[0m");
	while (tokens)
	{
		tok = tokens->content;
		if (tok->type == WORD)
			ft_printf("\033[1;33mword: %s\n\033[0m", tok->val);
		else if (tok->type == PIPE)
			ft_printf("\033[1;33mpipe\n\033[0m");
		else if (tok->type == REDIR_L)
			ft_printf("\033[1;33m<\n\033[0m");
		else if (tok->type == REDIR_LL)
			ft_printf("\033[1;33m<<\n\033[0m");
		else if (tok->type == REDIR_R)
			ft_printf("\033[1;33m>\n\033[0m");
		else if (tok->type == REDIR_RR)
			ft_printf("\033[1;33m>>\n\033[0m");
		else
			ft_printf("\033[1;33mtoken has invalid type\n\033[0m");
		tokens = tokens->next;
	}
}

void	print_redir(void *ptr)
{
	t_redir	*redir;

	redir = ptr;
	if (!redir)
		return ;
	if (redir->type == REDIR_RR)
		ft_printf(">> %s\n", redir->filename);
	else if (redir->type == REDIR_R)
		ft_printf("> %s\n", redir->filename);
	else if (redir->type == REDIR_LL)
		ft_printf("<< %s\n", redir->hd_keyword);
	else if (redir->type == REDIR_L)
		ft_printf("< %s\n", redir->filename);
	else
		ft_printf("redir has invalid type\n");
}

void	print_cmd(void *ptr)
{
	t_cmd	*cmd;

	cmd = ptr;
	if (!cmd)
		return ;
	ft_printf("\033[1;33mNEW CMD\n\033[0m");
	ft_printf("\033[1;33m--- args ---\n\033[0m");
	print_strs(cmd->args);
	ft_printf("\033[1;33m--- redirs ---\n\033[0m");
	ft_lstiter(cmd->redirs, &print_redir);
}

void	print_strs(char **strs)
{
	int	i;

	if (!strs)
	{
		printf("NULL\n");
		return ;
	}
	i = -1;
	while (strs[++i])
		printf("%-3d => |%s|\n", i, strs[i]);
	printf("%-3d => |NULL|\n", i);
}
