#ifndef AVACONTROLLER_H
#define AVACONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

char *openAVASession(const char *url);
void closeAVASession(void);
const char *getAVAProxyUrl(void);
const char *getAVAExtendedHttpHeader(void);

#ifdef __cplusplus
}
#endif

#endif
