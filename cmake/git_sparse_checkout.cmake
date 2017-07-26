# cmake module resposible for downloading required part of repository
#   GIT_REPO        - url to git repository that is required
#   GIT_REPO_DIR    - relative path to required part of repository

execute_process(COMMAND git init)
execute_process(COMMAND git remote add origin ${GIT_REPO})
execute_process(COMMAND git config core.sparsecheckout true)
set(sparse_directories ${GIT_REPO_DIR}/*)
FILE(APPEND .git/info/sparse-checkout ${sparse_directories})
execute_process(COMMAND git pull --depth=100 origin master)
if(GIT_TAG)
  execute_process(COMMAND git checkout ${GIT_TAG})
endif()
