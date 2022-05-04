# minishell

## Description

## Documentation

* Org repo: https://github.com/Shell-s-Heel/minishell

## Ressources
### Doc
* https://www.geeksforgeeks.org/fork-system-call/
* https://www.geeksforgeeks.org/wait-system-call-c/
* https://www.geeksforgeeks.org/signals-c-language/
* https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
* https://www.thegeekstuff.com/2012/03/linux-signals-fundamentals/
* https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
* https://www.geeksforgeeks.org/dup-dup2-linux-system-call/?ref=rp
* https://www.geeksforgeeks.org/pipe-system-call/?ref=rp
* https://www.tutorialspoint.com/unix/unix-signals-traps.htm
* https://www.tutorialspoint.com/unix_system_calls/execve.htm
* http://www.rozmichelle.com/pipes-forks-dups/
* http://tzimmermann.org/2017/08/17/file-descriptors-during-fork-and-exec/
* http://tzimmermann.org/2017/09/01/the-internals-of-unix-pipes-and-fifos/
* https://catonmat.net/ftp/bash-redirections-cheat-sheet.pdf
* http://minirighi.sourceforge.net/html/errno_8h-source.html
* https://tldp.org/LDP/abs/html/exitcodes.html
* https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/utilities/V3_chap02.html
* https://linuxhint.com/bash-environment-variables/

### Testers
* https://github.com/cacharle/minishell_test
* https://github.com/thallard/minishell_tester
* https://raw.githubusercontent.com/matboivin/ft_sh/main/tests/test_list.txt

### Workflow
* https://gist.github.com/Chaser324/ce0505fbed06b947d962
* https://youtu.be/deEYHVpE1c8 

## TODO

### Workflow

* Use fork and MR to contribute to the project

#### Git workflow :point_down:


On working branch, commit changes I want to have on the main branch:
```sh
$ git add
$ git commit -m "message"
$ git checkout main     //go to main to fetch this commit
```
pick-up commit made from working branch to main branch
```sh
$ git reflog            //copy your commit ID
$ git cherry-pick ID    //paste the ID
```
Before pushing & pull request, update your local folders
```sh
$ git fetch upstream        //fetch upstream data
$ git merge upstream/main   //merge data I fetched (apply changes)
$ gco working_branch        //go back on the working branch
$ git rebase main           //apply update to working branch
```
Then push to your repo & make a pull request
```sh
$ gco main            //go back to main branch
$ git push            //push your updated folder + ur new commits
```
Then, go to your github personnal repo to ask for a pull request to the general repo.



### Mandatory

* Write description and documentation
* Write the main loop and exit buitin
* Write the env, export and unset builtins
* Execute binaries with fork() and execve()
* Builtin cd, pwd, echo and resolve PATH in execve() (see getcwd() chdir() and stat())
* Write a lexer/parser (Atree? double linked list?)
* improve env variable to work within the parser.

### Optionnal

* `<<` redirection
* termcaps
