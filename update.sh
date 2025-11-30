git init
git config --global user.email "matrawarin@gmail.com"
git config --global user.name "matrawarin"
git add .
git add --all
git status
git commit -am 'matrawarin@gmail.com'
git remote set-url streaming-api 'https://github.com/matrawarin/streaming-api.git'
git remote add streaming-api 'https://github.com/matrawarin/streaming-api.git'
git push -u streaming-api main

