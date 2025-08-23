#ifndef DIAGNOSTIC_EXPORT_H
#define DIAGNOSTIC_EXPORT_H


#ifdef WIN
#ifdef DIAGNOSTIC_EXPORTS
#define DIAGNOSTIC_EXPORT __declspec(dllexport)
#else
#define DIAGNOSTIC_EXPORT __declspec(dllimport)
#endif
#else
#define DIAGNOSTIC_EXPORT __attribute__((visibility("default")))
#endif
#endif