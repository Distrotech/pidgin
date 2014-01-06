/*
 * This file was generated automatically by ExtUtils::ParseXS version 3.16 from the
 * contents of PluginPref.xs. Do not edit this file, edit PluginPref.xs instead.
 *
 *    ANY CHANGES MADE HERE WILL BE LOST!
 *
 */

#line 1 "PluginPref.xs"
#include "module.h"

#line 13 "PluginPref.c"
#ifndef PERL_UNUSED_VAR
#  define PERL_UNUSED_VAR(var) if (0) var = var
#endif

#ifndef dVAR
#  define dVAR		dNOOP
#endif


/* This stuff is not part of the API! You have been warned. */
#ifndef PERL_VERSION_DECIMAL
#  define PERL_VERSION_DECIMAL(r,v,s) (r*1000000 + v*1000 + s)
#endif
#ifndef PERL_DECIMAL_VERSION
#  define PERL_DECIMAL_VERSION \
	  PERL_VERSION_DECIMAL(PERL_REVISION,PERL_VERSION,PERL_SUBVERSION)
#endif
#ifndef PERL_VERSION_GE
#  define PERL_VERSION_GE(r,v,s) \
	  (PERL_DECIMAL_VERSION >= PERL_VERSION_DECIMAL(r,v,s))
#endif
#ifndef PERL_VERSION_LE
#  define PERL_VERSION_LE(r,v,s) \
	  (PERL_DECIMAL_VERSION <= PERL_VERSION_DECIMAL(r,v,s))
#endif

/* XS_INTERNAL is the explicit static-linkage variant of the default
 * XS macro.
 *
 * XS_EXTERNAL is the same as XS_INTERNAL except it does not include
 * "STATIC", ie. it exports XSUB symbols. You probably don't want that
 * for anything but the BOOT XSUB.
 *
 * See XSUB.h in core!
 */


/* TODO: This might be compatible further back than 5.10.0. */
#if PERL_VERSION_GE(5, 10, 0) && PERL_VERSION_LE(5, 15, 1)
#  undef XS_EXTERNAL
#  undef XS_INTERNAL
#  if defined(__CYGWIN__) && defined(USE_DYNAMIC_LOADING)
#    define XS_EXTERNAL(name) __declspec(dllexport) XSPROTO(name)
#    define XS_INTERNAL(name) STATIC XSPROTO(name)
#  endif
#  if defined(__SYMBIAN32__)
#    define XS_EXTERNAL(name) EXPORT_C XSPROTO(name)
#    define XS_INTERNAL(name) EXPORT_C STATIC XSPROTO(name)
#  endif
#  ifndef XS_EXTERNAL
#    if defined(HASATTRIBUTE_UNUSED) && !defined(__cplusplus)
#      define XS_EXTERNAL(name) void name(pTHX_ CV* cv __attribute__unused__)
#      define XS_INTERNAL(name) STATIC void name(pTHX_ CV* cv __attribute__unused__)
#    else
#      ifdef __cplusplus
#        define XS_EXTERNAL(name) extern "C" XSPROTO(name)
#        define XS_INTERNAL(name) static XSPROTO(name)
#      else
#        define XS_EXTERNAL(name) XSPROTO(name)
#        define XS_INTERNAL(name) STATIC XSPROTO(name)
#      endif
#    endif
#  endif
#endif

/* perl >= 5.10.0 && perl <= 5.15.1 */


/* The XS_EXTERNAL macro is used for functions that must not be static
 * like the boot XSUB of a module. If perl didn't have an XS_EXTERNAL
 * macro defined, the best we can do is assume XS is the same.
 * Dito for XS_INTERNAL.
 */
#ifndef XS_EXTERNAL
#  define XS_EXTERNAL(name) XS(name)
#endif
#ifndef XS_INTERNAL
#  define XS_INTERNAL(name) XS(name)
#endif

/* Now, finally, after all this mess, we want an ExtUtils::ParseXS
 * internal macro that we're free to redefine for varying linkage due
 * to the EXPORT_XSUB_SYMBOLS XS keyword. This is internal, use
 * XS_EXTERNAL(name) or XS_INTERNAL(name) in your code if you need to!
 */

#undef XS_EUPXS
#if defined(PERL_EUPXS_ALWAYS_EXPORT)
#  define XS_EUPXS(name) XS_EXTERNAL(name)
#else
   /* default to internal */
#  define XS_EUPXS(name) XS_INTERNAL(name)
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE assert(cv); assert(params)

/* prototype to pass -Wmissing-prototypes */
STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params);

STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params)
{
    const GV *const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char *const gvname = GvNAME(gv);
        const HV *const stash = GvSTASH(gv);
        const char *const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak(aTHX_ "Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak(aTHX_ "Usage: %s(%s)", gvname, params);
    } else {
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak(aTHX_ "Usage: CODE(0x%"UVxf")(%s)", PTR2UV(cv), params);
    }
}
#undef  PERL_ARGS_ASSERT_CROAK_XS_USAGE

#ifdef PERL_IMPLICIT_CONTEXT
#define croak_xs_usage(a,b)    S_croak_xs_usage(aTHX_ a,b)
#else
#define croak_xs_usage        S_croak_xs_usage
#endif

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) (PL_Sv=(SV*)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV*)PL_Sv)
#endif /* !defined(newXS_flags) */

#line 155 "PluginPref.c"

XS_EUPXS(XS_Purple__PluginPref__Frame_add); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref__Frame_add)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "frame, pref");
    {
	Purple__PluginPref__Frame	frame = purple_perl_ref_object(ST(0))
;
	Purple__PluginPref	pref = purple_perl_ref_object(ST(1))
;

	purple_plugin_pref_frame_add(frame, pref);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref__Frame_destroy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref__Frame_destroy)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "frame");
    {
	Purple__PluginPref__Frame	frame = purple_perl_ref_object(ST(0))
;

	purple_plugin_pref_frame_destroy(frame);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref__Frame_get_prefs); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref__Frame_get_prefs)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "frame");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	Purple__PluginPref__Frame	frame = purple_perl_ref_object(ST(0))
;
#line 46 "PluginPref.xs"
	GList *l;
#line 204 "PluginPref.c"
#line 48 "PluginPref.xs"
	for (l = purple_plugin_pref_frame_get_prefs(frame); l != NULL; l = l->next) {
		XPUSHs(sv_2mortal(purple_perl_bless_object(l->data, "Purple::PluginPref")));
	}
#line 209 "PluginPref.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Purple__PluginPref__Frame_new); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref__Frame_new)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "class");
    {
	Purple__PluginPref__Frame	RETVAL;

	RETVAL = purple_plugin_pref_frame_new(/* void */);
	ST(0) = purple_perl_bless_object(RETVAL, "Purple::PluginPref::Frame");
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_add_choice); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_add_choice)
{
    dVAR; dXSARGS;
    if (items != 3)
       croak_xs_usage(cv,  "pref, label, choice");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	const char *	label = (const char *)SvPV_nolen(ST(1))
;
	gpointer	choice = (SvPOKp(ST(2)) ? SvPVutf8_nolen(ST(2)) : (SvIOKp(ST(2)) ? GINT_TO_POINTER(SvIV(ST(2))) : NULL));

	purple_plugin_pref_add_choice(pref, label, choice);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_destroy); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_destroy)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;

	purple_plugin_pref_destroy(pref);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_get_bounds); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_bounds)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	int	min;
	int	max;

	purple_plugin_pref_get_bounds(pref, &min, &max);
	XSprePUSH;	EXTEND(SP,2);
	PUSHs(sv_newmortal());
	sv_setiv(ST(0), (IV)min);
	PUSHs(sv_newmortal());
	sv_setiv(ST(1), (IV)max);
    }
    XSRETURN(2);
}


XS_EUPXS(XS_Purple__PluginPref_get_choices); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_choices)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
#line 88 "PluginPref.xs"
	GList *l;
#line 304 "PluginPref.c"
#line 90 "PluginPref.xs"
	for (l = purple_plugin_pref_get_choices(pref); l != NULL; l = l->next) {
		XPUSHs(sv_2mortal(purple_perl_bless_object(l->data, "Purple::ListItem")));
	}
#line 309 "PluginPref.c"
	PUTBACK;
	return;
    }
}


XS_EUPXS(XS_Purple__PluginPref_get_label); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_label)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	const char *	RETVAL;
	dXSTARG;

	RETVAL = purple_plugin_pref_get_label(pref);
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_get_masked); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_masked)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	gboolean	RETVAL;

	RETVAL = purple_plugin_pref_get_masked(pref);
	ST(0) = boolSV(RETVAL);
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_get_format_type); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_format_type)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	Purple__String__Format__Type	RETVAL;
	dXSTARG;

	RETVAL = purple_plugin_pref_get_format_type(pref);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_get_max_length); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_max_length)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	unsigned int	RETVAL;
	dXSTARG;

	RETVAL = purple_plugin_pref_get_max_length(pref);
	XSprePUSH; PUSHu((UV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_get_name); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_name)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	const char *	RETVAL;
	dXSTARG;

	RETVAL = purple_plugin_pref_get_name(pref);
	sv_setpv(TARG, RETVAL); XSprePUSH; PUSHTARG;
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_get_type); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_get_type)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "pref");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	Purple__PluginPrefType	RETVAL;
	dXSTARG;

	RETVAL = purple_plugin_pref_get_type(pref);
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_new); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_new)
{
    dVAR; dXSARGS;
    if (items != 1)
       croak_xs_usage(cv,  "class");
    {
	Purple__PluginPref	RETVAL;

	RETVAL = purple_plugin_pref_new(/* void */);
	ST(0) = purple_perl_bless_object(RETVAL, "Purple::PluginPref");
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_new_with_label); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_new_with_label)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "class, label");
    {
	const char *	label = (const char *)SvPV_nolen(ST(1))
;
	Purple__PluginPref	RETVAL;

	RETVAL = purple_plugin_pref_new_with_label(label);
	ST(0) = purple_perl_bless_object(RETVAL, "Purple::PluginPref");
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_new_with_name); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_new_with_name)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "class, name");
    {
	const char *	name = (const char *)SvPV_nolen(ST(1))
;
	Purple__PluginPref	RETVAL;

	RETVAL = purple_plugin_pref_new_with_name(name);
	ST(0) = purple_perl_bless_object(RETVAL, "Purple::PluginPref");
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_new_with_name_and_label); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_new_with_name_and_label)
{
    dVAR; dXSARGS;
    if (items != 3)
       croak_xs_usage(cv,  "class, name, label");
    {
	const char *	name = (const char *)SvPV_nolen(ST(1))
;
	const char *	label = (const char *)SvPV_nolen(ST(2))
;
	Purple__PluginPref	RETVAL;

	RETVAL = purple_plugin_pref_new_with_name_and_label(name, label);
	ST(0) = purple_perl_bless_object(RETVAL, "Purple::PluginPref");
	sv_2mortal(ST(0));
    }
    XSRETURN(1);
}


XS_EUPXS(XS_Purple__PluginPref_set_bounds); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_bounds)
{
    dVAR; dXSARGS;
    if (items != 3)
       croak_xs_usage(cv,  "pref, min, max");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	int	min = (int)SvIV(ST(1))
;
	int	max = (int)SvIV(ST(2))
;

	purple_plugin_pref_set_bounds(pref, min, max);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_label); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_label)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, label");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	const char *	label = (const char *)SvPV_nolen(ST(1))
;

	purple_plugin_pref_set_label(pref, label);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_masked); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_masked)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, mask");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	gboolean	mask = (bool)SvTRUE(ST(1))
;

	purple_plugin_pref_set_masked(pref, mask);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_format_type); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_format_type)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, format");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	Purple__String__Format__Type	format = (Purple__String__Format__Type)SvIV(ST(1))
;

	purple_plugin_pref_set_format_type(pref, format);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_max_length); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_max_length)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, max_length");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	unsigned int	max_length = (unsigned int)SvUV(ST(1))
;

	purple_plugin_pref_set_max_length(pref, max_length);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_name); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_name)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, name");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	const char *	name = (const char *)SvPV_nolen(ST(1))
;

	purple_plugin_pref_set_name(pref, name);
    }
    XSRETURN_EMPTY;
}


XS_EUPXS(XS_Purple__PluginPref_set_type); /* prototype to pass -Wmissing-prototypes */
XS_EUPXS(XS_Purple__PluginPref_set_type)
{
    dVAR; dXSARGS;
    if (items != 2)
       croak_xs_usage(cv,  "pref, type");
    {
	Purple__PluginPref	pref = purple_perl_ref_object(ST(0))
;
	Purple__PluginPrefType	type = (Purple__PluginPrefType)SvIV(ST(1))
;
#line 177 "PluginPref.xs"
	PurplePluginPrefType gpp_type;
#line 629 "PluginPref.c"
#line 179 "PluginPref.xs"
	gpp_type = PURPLE_PLUGIN_PREF_NONE;

	if (type == 1) {
		gpp_type = PURPLE_PLUGIN_PREF_CHOICE;
	} else if (type == 2) {
		gpp_type = PURPLE_PLUGIN_PREF_INFO;
	} else if (type == 3) {
		gpp_type = PURPLE_PLUGIN_PREF_STRING_FORMAT;
	}
	purple_plugin_pref_set_type(pref, gpp_type);
#line 641 "PluginPref.c"
    }
    XSRETURN_EMPTY;
}

#ifdef __cplusplus
extern "C"
#endif
XS_EXTERNAL(boot_Purple__PluginPref); /* prototype to pass -Wmissing-prototypes */
XS_EXTERNAL(boot_Purple__PluginPref)
{
    dVAR; dXSARGS;
#if (PERL_REVISION == 5 && PERL_VERSION < 9)
    char* file = __FILE__;
#else
    const char* file = __FILE__;
#endif

    PERL_UNUSED_VAR(cv); /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
    XS_VERSION_BOOTCHECK;

        (void)newXSproto_portable("Purple::PluginPref::Frame::add", XS_Purple__PluginPref__Frame_add, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::Frame::destroy", XS_Purple__PluginPref__Frame_destroy, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::Frame::get_prefs", XS_Purple__PluginPref__Frame_get_prefs, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::Frame::new", XS_Purple__PluginPref__Frame_new, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::add_choice", XS_Purple__PluginPref_add_choice, file, "$$$");
        (void)newXSproto_portable("Purple::PluginPref::destroy", XS_Purple__PluginPref_destroy, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_bounds", XS_Purple__PluginPref_get_bounds, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_choices", XS_Purple__PluginPref_get_choices, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_label", XS_Purple__PluginPref_get_label, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_masked", XS_Purple__PluginPref_get_masked, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_format_type", XS_Purple__PluginPref_get_format_type, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_max_length", XS_Purple__PluginPref_get_max_length, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_name", XS_Purple__PluginPref_get_name, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::get_type", XS_Purple__PluginPref_get_type, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::new", XS_Purple__PluginPref_new, file, "$");
        (void)newXSproto_portable("Purple::PluginPref::new_with_label", XS_Purple__PluginPref_new_with_label, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::new_with_name", XS_Purple__PluginPref_new_with_name, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::new_with_name_and_label", XS_Purple__PluginPref_new_with_name_and_label, file, "$$$");
        (void)newXSproto_portable("Purple::PluginPref::set_bounds", XS_Purple__PluginPref_set_bounds, file, "$$$");
        (void)newXSproto_portable("Purple::PluginPref::set_label", XS_Purple__PluginPref_set_label, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::set_masked", XS_Purple__PluginPref_set_masked, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::set_format_type", XS_Purple__PluginPref_set_format_type, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::set_max_length", XS_Purple__PluginPref_set_max_length, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::set_name", XS_Purple__PluginPref_set_name, file, "$$");
        (void)newXSproto_portable("Purple::PluginPref::set_type", XS_Purple__PluginPref_set_type, file, "$$");

    /* Initialisation Section */

#line 7 "PluginPref.xs"
{
	HV *string_format_stash = gv_stashpv("Purple::String::Format::Type", 1);
	HV *plugin_pref_stash = gv_stashpv("Purple::PluginPref::Type", 1);

	static const constiv *civ, string_format_const_iv[] = {
#define const_iv(name) {#name, (IV)PURPLE_STRING_FORMAT_TYPE_##name}
		const_iv(NONE),
		const_iv(MULTILINE),
		const_iv(HTML),
	};
	static const constiv plugin_pref_const_iv[] = {
#undef const_iv
#define const_iv(name) {#name, (IV)PURPLE_PLUGIN_PREF_##name}
		const_iv(NONE),
		const_iv(CHOICE),
		const_iv(INFO),
		const_iv(STRING_FORMAT),
	};

	for (civ = string_format_const_iv + sizeof(string_format_const_iv) / sizeof(string_format_const_iv[0]); civ-- > string_format_const_iv; )
		newCONSTSUB(string_format_stash, (char *)civ->name, newSViv(civ->iv));

	for (civ = plugin_pref_const_iv + sizeof(plugin_pref_const_iv) / sizeof(plugin_pref_const_iv[0]); civ-- > plugin_pref_const_iv; )
		newCONSTSUB(plugin_pref_stash, (char *)civ->name, newSViv(civ->iv));
}

#line 721 "PluginPref.c"

    /* End of Initialisation Section */

#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}

