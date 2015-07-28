#!/bin/sh

if [ "$SUITE" = "build" ]; then
    ## the following automatically builds the doxygen
    ## documentation and pushes it to the gh_pages branch

    # First, set up credentials using the environment variables
    # GIT_NAME, GIT_EMAIL and GH_TOKEN. These were passed
    # encrypted to travis and should have been decrypted
    # using travis' private key before this script was run.
    git config --global user.name $GIT_NAME
    git config --global user.email $GIT_EMAIL

    # clone the whole repo again, but switch to gh_pages branch
    mkdir full
    git clone https://github.com/poppy-project/poppy-com.git full
    cd full
    make docs
    git checkout gh-pages

    # cd into the docs dir and commit and push the new docs.
    git status
    git add Docs/*
    git commit -m "Auto-updating $TRAVIS_REPO_SLUG documentation"
    git push https://$GH_TOKEN@github.com/$TRAVIS_REPO_SLUG gh-pages

elif [ "$SUITE" = "tests" ]; then
    coveralls --exclude stdio2 -e test -e test_mngmnt -e hal --gcov-options '\-lp'


else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
