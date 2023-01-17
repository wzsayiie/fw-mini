#pragma once

#define mp_window_title          _mp_window_title()

#define mp_resbundle_folder_name _mp_resbundle_folder_name()
#define mp_document_folder_name  _mp_document_folder_name()
#define mp_temporary_folder_name _mp_temporary_folder_name()

#define mp_enable_javascript     _mp_enable_javascript()

#define mp_log_to_file           _mp_log_to_file()
#define mp_log_file_name         _mp_log_file_name()

const char *_mp_window_title         ();
const char *_mp_resbundle_folder_name();
const char *_mp_document_folder_name ();
const char *_mp_temporary_folder_name();
bool        _mp_enable_javascript    ();
bool        _mp_log_to_file          ();
const char *_mp_log_file_name        ();
