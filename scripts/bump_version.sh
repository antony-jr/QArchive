#!/usr/bin/env bash

echo "Bump Version, Simple Script to Bump QArchive Version"
echo "Copyright (c) 2023-Preset, D. Antony J.R <contact@antonyjr.in>"
echo ""

if [[ -z "$QARCHIVE_VERSION" ]] then
   echo "QARCHIVE_VERSION not set, giving up."
   exit -1
fi

echo QARCHIVE_VERSION=$QARCHIVE_VERSION

sed -i 's/QArchive VERSION .*/QArchive VERSION '$QARCHIVE_VERSION'\)/' CMakeLists.txt
sed -i "s/  version: .*/  version: '$QARCHIVE_VERSION',/" meson.build

cat > /tmp/qarchive.commit.template.txt << EOF
[Release $QARCHIVE_VERSION] QArchive v$QARCHIVE_VERSION release

#Changelog

   * Change 1
   * Change 2

Signed-off-by: Divya Antony J R <antonyjr@protonmail.com>
EOF

git add -A
git status

read -p "Do you want to commit? press any key to continue..."

git commit -S -t /tmp/qarchive.commit.template.txt

rm -rf /tmp/qarchive.commit.template.txt

echo "Please run 'git push origin master' to release."
