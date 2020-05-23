---
tags:
	- tag
	- git
  - deployment
---
```bash
git tag -a v2.0b6 -m "this is a test for 2.0b6 release"
git push --tags

git tag -d v2.0b6
git push --delete origin v2.0b6

export DRONE_TAG=v2.0b6
```
