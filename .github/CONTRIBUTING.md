# Contribution Guide

QArchive is a small library that modernizes **libarhive** using C++ and Qt5 , So you need to go through libarchive's   
**[documentaion](https://github.com/libarchive/libarchive/wiki)** and **[Qt Docs](https://doc.qt.io/qt-5/)**.   
You **don't need to be an expert** in any of these because everyone is learning and I specifically want this project to welcome   
new contributors and let them know how a open source project works. All contributors and those who wish to contribute must follow the **[code of conduct](CODE_OF_CONDUCT.md)**. Thats the **basics** that you need to know to **get started!**.


### Reporting Issues

Before you **open an issue** make sure you have no **duplicates**. **Feel free to open an issue to ask anything** but make   
sure you follow the **issue template** which will not be that hard. Be specific about your issue if its a **bug** or a   
**feature request** to make it easy for the developers to implement it. And last but not the least , **Thank You for your Time!** :heart:

### Developing QArchive

If you wish to develop some **code** for the project then you are much **welcomed** but make sure you **open an issue first** to   
describe the cause why we need to improve the code after that **open a pull request** and also make sure to **follow the pull request template**.    And you need to follow the following **rules when updating the source files**.

 * You **must** follow the **[linux kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)**.
 * You **can** use **[Artistic Styles](http://astyle.sourceforge.net/)** to automatically format your code to follow the
   **linux kernel coding style**.   
   ```
    $ astyle --style=linux src/*cc include/*hpp
    $ rm src/*orig include/*orig # remove old files.
   ```
   
 * You **must** follow **google's** conventions for c++ file extensions , (i.e) for **source file(s)** you must use **.cc**
   extension and for **header file(s)** you must use **.hpp**.
 * Use comments only to write why we need a **class** or a **function** and not **what**.
 * If you create a new method then you **must** be consistent as other methods.
 * You **can** do whatever with **private slots** and can return any return type if you desire , but make sure it is efficient.
 * **Update the docs** if you add any new method to a **class**.
 * **Do not** add **unwanted file(s)** and doing so will make your **pull request invalid**.
 
After making changes to the source file(s) , open a pull request and give a **good title** and **mention** the **issue you opened before**. Thats it! And last but not the least , **Thank You very much for Contributing!** :heart:

### Reference

libarchive - https://github.com/libarchive/libarchive   
linux kernel coding style - https://www.kernel.org/doc/html/v4.10/process/coding-style.html   
astyle - http://astyle.sourceforge.net/   
Qt Documentation - https://doc.qt.io/qt-5/
