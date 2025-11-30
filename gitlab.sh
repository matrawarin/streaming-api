git init
git config --global user.email "songkeart@gmail.com"
git config --global user.name "lexneconst-project"
git add .
git add --all
git status
git remote add gitlab 'https://gitlab.com/lexneconst-group/lexneconst-project.git'
git branch -M main
git push -uf gitlab main
