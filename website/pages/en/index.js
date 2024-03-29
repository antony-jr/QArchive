/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const React = require('react');

const CompLibrary = require('../../core/CompLibrary.js');
const MarkdownBlock = CompLibrary.MarkdownBlock; /* Used to read markdown */
const Container = CompLibrary.Container;
const GridBlock = CompLibrary.GridBlock;

const siteConfig = require(process.cwd() + '/siteConfig.js');

class Button extends React.Component {
  render() {
    return (
      <div className="pluginWrapper buttonWrapper" id={this.props.key}>
        <a className="button" href={this.props.href} target={this.props.target}>
          {this.props.children}
        </a>
      </div>
    );
  }
}

Button.defaultProps = {
  target: '_self',
};

class HomeSplash extends React.Component {
  render() {
    return (
      <div className="homeContainer">
        <div className="homeSplashFade">
          <div className="wrapper homeWrapper">
            <div className="projectLogo">
              <img src={siteConfig.baseUrl + 'img/QArchive.png'} />
            </div>
            <div className="inner">
              <h2 className="projectTitle">
                {siteConfig.title}
                <small>{siteConfig.tagline}</small>
              </h2>
              <div className="section promoSection">
                <div className="promoRow">
                  <div className="pluginRowBlock">
                    <Button
                      href={
                        siteConfig.baseUrl +
                        'docs/' +
                        'Installation.html'
                      }>
                      Get Started
                    </Button>
		    {/*
		      It's really irritating to get PVS Studio
		      license every year by requesting license,
		      must move on to something open source.
		      
		      <Button
                      href={
                        siteConfig.baseUrl +
                        'PVSStudioAnalysis/' +
                        'index.html'
                      }>
                      PVS Studio Report
		    </Button>*/}
	            <Button href={'https://github.com/antony-jr/QArchive'}>
	    	      View on Github
	    	    </Button>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

class Index extends React.Component {
  render() {
    let language = this.props.language || 'en';
    const showcase = siteConfig.users
      .filter(user => {
        return user.pinned;
      })
      .map(user => {
        return (
          <a href={user.infoLink}>
            <img src={user.image} title={user.caption} />
          </a>
        );
      });

    return (
      <div>
        <HomeSplash language={language} />
        <div className="mainContainer">
          <Container padding={['bottom', 'top']}>
            <GridBlock
              align="center"
              contents={[
                {
                  content: 'QArchive fully supports Qt Projects by flowing with its event loop and'+
			   ' thus you will have no problem integrating QArchive with your Qt Project.'+
			   'The whole library is also single threaded and non-blocking but you can also'+
			   ' run it in a different thread too.',
                  image: siteConfig.baseUrl + 'img/qt.png',
                  imageAlign: 'top',
                  title: 'Supports Qt\'s Event Loop.',
                },
                {
                  content: 'Whatever archive you throw at QArchive, it will never fail you Because its' +
			   ' backed by a very stable project ( libarchive ) and so giving you a lot of options '+
			   'with a clean C++ API.',
                  image: siteConfig.baseUrl + 'img/extract.png',
                  imageAlign: 'top',
                  title: 'Works with a Wide Range of Archive Formats.',
                },
		{
                  content: 'QArchive does not like leaving heavy traces for the users and thus '+
                           'it is packed with all its amazing features in a very small C++ library, this ' +
                           'makes QArchive easier to integrate with your project.',
                  image: siteConfig.baseUrl + 'img/light.png',
                  imageAlign: 'top',
                  title: 'Light Weight!',
                },
		{
                  content: 'This project is rated by <b>Codacy</b> with a <b>A Project Certification</b>' +
                           ' and thus integrating this header will not affect your source, So this project is also'+
                           ' best suited for those who care about code taste.',
                  image: siteConfig.baseUrl + 'img/clean_code.png',
                  imageAlign: 'top',
                  title: 'Clean C++ API.',
                },

              ]}
              layout="fourColumn"
            />
          </Container>

          <div className="productShowcaseSection paddingBottom">
            <h2>{"Who's Using This?"}</h2>
            <p>This project is used by all these people</p>
            <div className="logos">{showcase}</div>
            <div className="more-users">
              <a
                className="button"
                href={
                  siteConfig.baseUrl + this.props.language + '/' + 'users.html'
                }>
                More {siteConfig.title} Users
              </a>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

module.exports = Index;
