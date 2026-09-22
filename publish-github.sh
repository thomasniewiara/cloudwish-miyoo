#!/bin/sh
set -eu
cd "$(dirname "$0")"
repo=thomasniewiara/cloudwish-miyoo
gh auth status
if ! gh repo view "$repo" >/dev/null 2>&1; then
  gh repo create "$repo" --private --description "Cloudwish Kingdom native Miyoo Mini Plus / Onion OS game, with all release archives"
fi
if git remote get-url origin >/dev/null 2>&1; then
  test "$(git remote get-url origin)" = "https://github.com/$repo.git" || { echo "Unexpected origin; refusing to push."; exit 1; }
else
  git remote add origin "https://github.com/$repo.git"
fi
gh auth setup-git
git push origin main --tags
for v in 0.1 0.1.1 0.1.2 0.2 0.3; do
  if ! gh release view "v$v" --repo "$repo" >/dev/null 2>&1; then
    gh release create "v$v" --repo "$repo" --verify-tag --title "Cloudwish Miyoo v$v" --notes-file "releases/v$v/CHANGELOG.md" "releases/v$v/"*.zip "releases/v$v/SHA256SUMS.txt"
  fi
done
