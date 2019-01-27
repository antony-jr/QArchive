/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* List of projects/orgs using your project for the users page */

const users = [];

const siteConfig = {
  title: 'QArchive' /* title for your website */,
  tagline: 'A Simple Archiver for Qt Projects.',
  url: 'https://antony-jr.github.io' /* your website url */,
  baseUrl: '/QArchive/' /* base url for your project */,
  projectName: 'QArchive',
  headerLinks: [
    {doc: 'Installation', label: 'Docs'},
    {page: 'help', label: 'Help'},
    {blog: true, label: 'Blog'},
  ],
  users,
  /* path to images for header/footer */
  headerIcon: 'img/QArchive.png',
  footerIcon: 'img/QArchive.png',
  favicon: 'img/favicon.png',
  /* colors for website */
  colors: {
    primaryColor: '#262626',
    secondaryColor: '#1a8cff',
  },
  // This copyright info is used in /core/Footer.js and blog rss/atom feeds.
  copyright:
    'Copyright © ' +
    new Date().getFullYear() +
    ' Antony Jr.',
  organizationName: 'antony-jr', // or set an env variable ORGANIZATION_NAME
  highlight: {
    // Highlight.js theme to use for syntax highlighting in code blocks
    theme: 'default',
  },
  scripts: ['https://buttons.github.io/buttons.js'],
  // You may provide arbitrary config keys to be used as needed by your template.
  repoUrl: 'https://github.com/antony-jr/QArchive',
};

module.exports = siteConfig;
