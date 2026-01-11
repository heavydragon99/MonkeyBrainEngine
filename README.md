# MonkeyBrainEngine
A c++ game engine which will probably never actually be made


MONKEHHHHHHH

# rebase
How to rebase a feature branch onto the main branch
## 1 Rebase feature branch
```bash
git checkout your-branch
git fetch origin
git rebase -i origin/main
```

## 2 Fix conflicts in files (if any)
```bash
git add .
git rebase --continue
```

## 3 Push the rebased branch
```bash
git push --force-with-lease
```

## 4 Merge back to main
```bash
git checkout main
git pull origin main
git merge your-branch
git push origin main
```


# TODO
## Siem
* Mess aroung with dynamic archetype/chunking class generation

## Sean
* search which components our engine should have
* Find out how to correctly use git
