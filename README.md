
<p align="center">
  <img src="artwork/logo_final.png" height="200px" width=auto alt="QArchive Logo">  <br>
</p>


# QArchive [![GitHub issues](https://img.shields.io/github/issues/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/issues) [![GitHub forks](https://img.shields.io/github/forks/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/network) [![GitHub stars](https://img.shields.io/github/stars/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/stargazers) [![GitHub license](https://img.shields.io/github/license/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/blob/master/LICENSE) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/1ebae88c4a4e4e9d9a494568799a9ec8)](https://www.codacy.com/app/antony-jr/QArchive?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=antony-jr/QArchive&amp;utm_campaign=Badge_Grade) [![Travis Build status](https://travis-ci.org/antony-jr/QArchive.svg?branch=master)](https://travis-ci.org/antony-jr/QArchive) [![Appveyor Build status](https://ci.appveyor.com/api/projects/status/a49bqd5qssojj841?svg=true)](https://ci.appveyor.com/project/antony-jr/qarchive)

QArchive is a cross-platform C++ library that modernizes libarchive , This library helps you to extract and compress 
archives supported by libarchive. The whole library itself is crafted to work perfectly well with the 
Qt event loop and thus its a perfect fit for your Qt projects.

I personally made this project to extract .7z archives in order to create a library that can update Qt Apps installed via
Qt Installer Framework without using the updater tool provided by Qt , because I hated using sub-process and besides
I wanted a in-built updater for my application and thats not possible with the already provided updater from 
Qt Installer Framework.

## Features

* *Single threaded and Non-Blocking API* - Using Qt's Event Loop. (**you can optionally run in a different thread**)

* *Easy to Use API* - Made some effort to match Qt's style.

* *Cyclic API , No use of mutex* - The whole library is fully cyclic and therefore no mutex is used.(Only signals and slots.)

* *Drink the Qt Kool-aid* - In a positive way.


**Witness it with your own eyes,**

```
#include <QCoreApplication>
#include <QArchive>

int main(int argc, char **argv)
{
    using QArchive::DiskExtractor;
    QCoreApplication app(argc, argv);
    DiskExtractor Extractor("Test.7z");
    
    /* Connect Signals and Slots. */
    QObject::connect(&Extractor , 
                     &DiskExtractor::finished , 
                     &app , 
                     &QCoreApplication::quit);
    
    Extractor.start();
    return app.exec();
}
```

# Getting Started

Learn more about **QArchive** at the official [documentation](https://antony-jr.github.io/QArchive).

# Contributors [![QArchive Contributors](https://img.shields.io/github/contributors/antony-jr/QArchive.svg)](https://github.com/antony-jr/QArchive/graphs/contributors)

My utmost **gratitude goes to these people!** :heart:

<table>
    <tr align="center">
        <td>
            <img src="https://avatars1.githubusercontent.com/u/1927154?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/ardeidae">Matthieu Petiot</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=ardeidae">💻</a>
        </td>
        <td>
            <img src="https://avatars1.githubusercontent.com/u/660896?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/adriweb">Adrien Bertrand</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=adriweb">💻</a>
        </td>
      <td>
            <img src="https://avatars1.githubusercontent.com/u/1274384?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/alberthdev">Albert Huang</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=alberthdev">💻</a>
        </td>
      <td>
            <img src="https://avatars3.githubusercontent.com/u/11664992?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/HadesD">Ichiro</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=HadesD">💻</a>
        </td>
    </tr>
</table>

You can also **become one of them by contributing to this project** , to know more read **[CONTRIBUTING.md](.github/CONTRIBUTING.md)**.


# Thank You ![Thank You](https://img.shields.io/badge/Always-Say%20Thank%20You!-blue.svg?style=flat-square)

I really need to thank the developers of this libraries for creating it because QArchive is elegant because of them! :heart:   

* [libarchive](https://github.com/libarchive/libarchive)
* [Qt](https://github.com/qt)


# Support [![Twitter](https://img.shields.io/twitter/url/https/github.com/antony-jr/QArchive.svg?style=social)](https://twitter.com/intent/tweet?text=Checkout%20%23QArchive%20by%20%40antonyjr0%20%20%2C%20its%20cool.%20Try%20it%20at%20https%3A%2F%2Fgithub.com%2Fantony-jr%2FQArchive)

If you think that this project is **cool** then you can give it a :star: or :fork_and_knife: it if you want to improve it with me. I really :heart: stars though!   

# License

The BSD 3-clause "New" or "Revised" License.

Copyright (C) 2017 , antony jr.   
All Rights Reserved.
