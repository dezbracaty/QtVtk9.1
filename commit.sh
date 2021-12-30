git add .
git rm --cached -r build/* CMakeLists.txt.user
git commit -m "$1"
git push origin VTK9.1
