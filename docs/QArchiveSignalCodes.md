---
id: QArchiveSignalCodes
title: QArchive Signal Codes
sidebar_label: QArchive Signal Codes
---

 These codes can be used in **'setFunc'** method that is common on   
 all classes in the **QArchive Namespace**.

 **Note:** Some overloads of **'setFunc'** does **not need signal codes**   
 because some signals have unique function prototype. 
 Example **error** signal **does not** require the **signal code**.  
 **Go through the classes documentations to know more**.

| Signal Code                         | Signal                                        |  Value  |
|-------------------------------------|-----------------------------------------------|---------|
| CANCELED                            | Denotes the cancel signal.                    |    0    |
| COMPRESSED                          | Denotes the compressed signal.                |    1    |
| COMPRESSING                         | Denotes the compressing signal.               |    2    |
| EXTRACTED                           | Denotes the extracted signal.                 |    3    |
| EXTRACTING                          | Denotes the extracting signal.                |    4    |
| FINISHED                            | Denotes the finished signal.                  |    5    |
| PASSWORD_REQUIRED                   | Denotes the password required signal.         |    6    |
| PROGRESS                            | Denotes the progress signal.                  |    7    |
| RESUMED                             | Denotes the resumed signal.                   |    8    |
| STARTED                             | Denotes the started signal.                   |    9    |
| PAUSED                              | Denotes the paused signal.                    |    10   |
