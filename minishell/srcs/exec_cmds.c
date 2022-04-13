#include "minishell.h"

char	**get_paths(char **local_env)
{
	char	**paths;
	char	*tmp;

	tmp = get_ev_value("PATH", local_env);
	paths = ft_split(tmp, ':');
	free(tmp);
	return (paths);
}

int	has_access(char *cmd_name)
{
	if (!access(cmd_name, F_OK | X_OK))
		return (1);
	return (0);
}

char	*get_cmd_path(char *cmd_name, char **local_env)
{
	char	*cmd_path;
	char	*cmd_pfx;
	char	**paths;
	int		i;

	if (ft_strchr(cmd_name, '/') && has_access(cmd_name))
		return (ft_strdup(cmd_name));
	if (ft_strchr(cmd_name, '/') && !has_access(cmd_name))
		return (NULL);
	cmd_path = NULL;
	paths = get_paths(local_env);
	cmd_pfx = ft_strjoin("/", cmd_name);
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd_pfx);
		if (!access(cmd_path, F_OK | X_OK))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	free_strs(paths);
	free(cmd_pfx);
	return (cmd_path);
}

int	is_builtin(char *cmd_name)
{
	if (!my_strcmp(cmd_name, "cd") || !my_strcmp(cmd_name, "echo")
		|| !my_strcmp(cmd_name, "pwd") || !my_strcmp(cmd_name, "export")
		|| !my_strcmp(cmd_name, "unset") || !my_strcmp(cmd_name, "env")
		|| !my_strcmp(cmd_name, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_var *var)
{
	if (!my_strcmp(args[0], "cd"))
		return (0);
	else if (!my_strcmp(args[0], "echo"))
		return (0);
	else if (!my_strcmp(args[0], "pwd"))
		return (0);
	else if (!my_strcmp(args[0], "export"))
		return (0);
	else if (!my_strcmp(args[0], "unset"))
		return (0);
	else if (!my_strcmp(args[0], "env"))
		return (0);
	else if (!my_strcmp(args[0], "exit"))
		return (0);
	return (0);
}

void	exec_in_chld(t_cmd *cmd, t_var *var, int pipe_fd[2])
{
	char	*cmd_path;
	pid_t	pid;

	if (cmd->args)
	{
		cmd_path = get_cmd_path(cmd->args[0], var->local_env);
		if (!cmd_path)
		{
			print_error(ft_strdup(cmd->args[0]), "command not found");
			var->exit_status = CMD_NOT_FOUND;
			return ;
		}
	}
	var->nb_chld++;
	pid = fork();
	if (pid == -1)
	{
		var->nb_chld--;
		print_error(ft_strdup("fork"), strerror(errno));
	}
	if (!pid)
	{
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		if (pipe_fd)
		{
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
		}
		if (redirect_stdout(cmd->redir_out))
			exit(EXIT_FAILURE);
		if (redirect_stdin(cmd->redir_in))
			exit(FILE_NOT_FOUND);
		if (!cmd->args)
			exit(EXIT_SUCCESS);
		if (is_builtin(cmd->args[0]))
			exit(exec_builtin(cmd->args, var));
		if (execve(cmd_path, cmd->args, var->local_env) == -1)
			print_error(ft_strdup("execve"), strerror(errno));
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
}

void	exec_simple_cmd(t_cmd *cmd, t_var *var)
{
	if (!cmd->args)
		return ;
	if (is_builtin(cmd->args[0]))
	{
		if (redirect_stdout(cmd->redir_out))
		{
			var->exit_status = FILE_NOT_FOUND;
			return ;
		}
		var->exit_status = exec_builtin(cmd->args, var);
	}
	else
		exec_in_chld(cmd, var, NULL);
}

void	exec_multiple_cmds(t_list *cmds, t_var *var)
{
	char	*cmd_path;
	int		pipe_fd[2];
	pid_t	pid;

	if (cmds->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			print_error(ft_strdup("pipe"), strerror(errno));
			return ;
		}
		exec_in_chld(cmds->content, var, pipe_fd);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		exec_multiple_cmds(cmds->next, var);
	}
	else
		exec_in_chld(cmds->content, var, NULL);
}

void	exec_cmds(t_list *cmds, t_var *var)
{
	if (!cmds)
		return ;
	if (handle_hd(cmds))
		return ;
	set_sig(SIGINT, SIG_IGN);
	reset_tty();
	if (cmds->next)
		exec_multiple_cmds(cmds, var);
	else
		exec_simple_cmd(cmds->content, var);
}