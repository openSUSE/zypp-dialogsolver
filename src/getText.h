/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	getText.h
 *
 * Interface to getText.
 *
*/
#ifndef DIALOGSOLVER_GETTEXT_H
#define DIALOGSOLVER_GETTEXT_H

/** Just tag text for translation. */
#define N_(MSG) QString(MSG)

/** Return translated text. */
#define _(MSG) QString::fromUtf8(::dialogsolver::dgettext( MSG ))

/** Return translated text. */
#define i18n(MSG) QString::fromUtf8(::dialogsolver::dgettext( MSG ))

/** Return translated text (plural form). */
#define _PL(MSG1,MSG2,N) QString::fromUtf8(::dialogsolver::dngettext( MSG1; MSG2, N ))

///////////////////////////////////////////////////////////////////
namespace dialogsolver
{ /////////////////////////////////////////////////////////////////

/** Return translated text. */
    const char * dgettext( const char * msgid );

/** Return translated text (plural form). */
    const char * dngettext( const char * msgid1, const char * msgid2,
			    unsigned long n );

    /////////////////////////////////////////////////////////////////
} // namespace dialogsolver

#endif // DIALOGSOLVER_GETTEXT_H
