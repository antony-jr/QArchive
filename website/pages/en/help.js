/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const React = require('react');

const CompLibrary = require('../../core/CompLibrary.js');
const Container = CompLibrary.Container;
const GridBlock = CompLibrary.GridBlock;

const siteConfig = require(process.cwd() + '/siteConfig.js');

class Help extends React.Component {
  render() {
    const supportLinks = [
      {
        content:
          'Learn more using the [documentation on this site.](/QArchive/docs/Installation.html)',
        title: 'Browse Docs',
      },
      {
        content: 'If you have any questions about the author or the project just ask me , get in touch with me on '+
	      	 '<a href="https://github.com/antony-jr/ama">Ask Me Anything!</a>',
        title: 'Ask the Author',
      },
      {
        content: "If you ever face any problem , please raise a <a href='https://github.com/antony-jr/QArchive/issues/new'>"+
	      	 "Issue</a> to get it fixed asap!",
        title: 'Open an Issue on GitHub',
      },
    ];

    return (
      <div className="docMainWrapper wrapper">
        <Container className="mainContainer documentContainer postContainer">
          <div className="post">
            <header className="postHeader">
              <h2>Need help?</h2>
            </header>
            <p>
	    This project is maintained by <a href="https://github.com/antony-jr">Antony Jr</a>.
	    </p>
            <GridBlock contents={supportLinks} layout="threeColumn" />
          </div>
        </Container>
      </div>
    );
  }
}

module.exports = Help;
