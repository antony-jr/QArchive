
<p align="center">
  <img src="artwork/logo_final.png" height="200px" width=auto alt="QArchive Logo">  <br>
</p>


# QArchive [![GitHub issues](https://img.shields.io/github/issues/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/issues) [![GitHub forks](https://img.shields.io/github/forks/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/network) [![GitHub stars](https://img.shields.io/github/stars/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/stargazers) [![GitHub license](https://img.shields.io/github/license/antony-jr/QArchive.svg?style=flat-square)](https://github.com/antony-jr/QArchive/blob/master/LICENSE) [![Test and Deploy](https://github.com/antony-jr/QArchive/actions/workflows/tests.yml/badge.svg?branch=master)](https://github.com/antony-jr/QArchive/actions/workflows/tests.yml)

QArchive is a cross-platform C++ library that modernizes libarchive , This library helps you to extract and compress 
archives supported by libarchive. The whole library itself is crafted to work perfectly well with the 
Qt event loop and thus its a perfect fit for your Qt projects.

I personally made this project to extract .7z archives in order to create a library that can update Qt Apps installed via
Qt Installer Framework without using the updater tool provided by Qt , because I hated using sub-process and besides
I wanted a in-built updater for my application and that's not possible with the already provided updater from 
Qt Installer Framework.

## Features

* *Single threaded and Non-Blocking API* - Using Qt's Event Loop. (**you can optionally run in a different thread**)

* *Easy to Use API* - Made some effort to match Qt's style.

* *Cyclic API , No use of mutex* - The whole library is fully cyclic and therefore no mutex is used.(Only signals and slots.)

* *Drink the Qt Kool-aid* - In a positive way.


**Witness it with your own eyes,**

```
#include <QCoreApplication>
#include <QArchive/QArchive>

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

The documentation does not document any new changes that happen at the master branch and will only have
information about the released version of the library. If you encounter API breaks in master branch, this
is intentional, please use the release version. **This situation will change soon in the future, so sorry
for the inconvenience.**

## Qt6 Support

Starting from QArchive **v2.2.3**, Qt6 support exists, if you have only Qt6 installed in the system then
QArchive's cmake should pick that up, if you have both Qt5 and Qt6, QArchive will pick Qt5, you can pass 
```-DQARCHIVE_QT_VERSION_MAJOR=6``` to cmake to force to build QArchive with Qt6 libs.

## Conan

*Thanks to [Eric Riff](https://github.com/ericriff)*.

Starting from version 2.0.1 `QArchive` is available on the [Conan C/C++ package manager](https://conan.io/), which means you can download prebuilt packages for all major platforms (Windows, Linux, macOS, etc).

Find an example on how to consume Conan's precompiled packages in the examples folder.

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
      <td>
            <img src="https://avatars1.githubusercontent.com/u/187720?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/longseespace">Long Nguyen</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=longseespace">💻</a>
        </td>
      <td>
            <img src="https://avatars2.githubusercontent.com/u/50304471?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/g-fb">g-fb</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=g-fb">💻</a>
        </td>
  </tr>
  <tr align="center">
         <td>
            <img src="https://avatars.githubusercontent.com/u/2084073?s=400&u=6278d48f6a8eb75af82395593f31af5a960cb95a&v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/dprotti">Duilio Protti</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=dprotti">💻</a>
        </td>
	<td>
            <img src="https://avatars.githubusercontent.com/u/57375845?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/ericriff">Eric Riff</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=ericriff">💻</a>
        </td>
	<td>
            <img src="https://avatars.githubusercontent.com/u/2747603?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/ZemingLiu">GGBond</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=ZemingLiu">💻</a>
        </td>
        <td>
            <img src="https://avatars.githubusercontent.com/u/2444564?v=4" width="100px"><br>
            <sub>
                <strong>
                    <a href="https://github.com/DeuxVis">DeuxVis</a>
                </strong>
            </sub><br>
            <a href="https://github.com/antony-jr/QArchive/commits?author=DeuxVis">💻</a>
        </td>
	<td>
	   <img src="https://avatars.githubusercontent.com/u/39350000?v=4" width="100px"><br>
	   <sub>
              <strong>
                 <a href="https://github.com/yosicovich">yosicovich</a>
	      </strong>
           </sub><br>
	   <a href="https://github.com/antony-jr/QArchive/commits?author=yosicovich">💻</a> 
        </td>
        <td> 
           <img src="https://avatars.githubusercontent.com/u/3681516?v=4" width="100px"><br> 
           <sub> 
              <strong> 
                 <a href="https://github.com/tastytea">tastytea</a> 
              </strong> 
           </sub><br> 
           <a href="https://github.com/antony-jr/QArchive/commits?author=tastytea">💻</a>  
        </td> 
  </tr>
  <tr align="center">
     <td>
       <img src="https://avatars.githubusercontent.com/u/11352254?v=4" width="100px"><br>
       <sub>
         <strong>
            <a href="https://github.com/lmussier">M. Ludo</a>
         </strong>
       </sub><br>
       <a href="https://github.com/antony-jr/QArchive/commits?author=lmussier">💻</a>
     </td>
     <td>
       <img src="https://avatars.githubusercontent.com/u/2918873?v=4" width="100px"><br>
       <sub>
         <strong>
            <a href="https://github.com/neheb">Rosen Penev</a>
         </strong>
       </sub><br>
       <a href="https://github.com/antony-jr/QArchive/commits?author=neheb">💻</a>
     </td>
     <td>
       <img src="https://avatars.githubusercontent.com/u/26579016?v=4" width="100px"><br>
       <sub>
         <strong>
            <a href="https://github.com/Jihadist">Anton</a>
         </strong>
       </sub><br>
       <a href="https://github.com/antony-jr/QArchive/commits?author=Jihadist">💻</a>
     </td>
  </tr> 
</table>

You can also **become one of them by contributing to this project** , to know more read **[CONTRIBUTING.md](.github/CONTRIBUTING.md)**.


# Thank You ![Thank You](https://img.shields.io/badge/Always-Say%20Thank%20You!-blue.svg?style=flat-square)

I really need to thank the developers of these libraries for creating it because QArchive is elegant because of them! :heart:   

* [libarchive](https://github.com/libarchive/libarchive)
* [Qt](https://github.com/qt)


# Support [![Twitter](https://img.shields.io/twitter/url/https/github.com/antony-jr/QArchive.svg?style=social)](https://twitter.com/intent/tweet?text=Checkout%20%23QArchive%20by%20%40antonyjr0%20%20%2C%20its%20cool.%20Try%20it%20at%20https%3A%2F%2Fgithub.com%2Fantony-jr%2FQArchive)

If you think that this project is **cool** then you can give it a :star: or :fork_and_knife: it if you want to improve it with me. I really :heart: stars though!   

# License

The BSD 3-clause "New" or "Revised" License.

Copyright (C) 2017-present, QArchive Contributors and Antony J.R

All Rights Reserved.
