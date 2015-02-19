
/*
	This is a udf_charsets.c file with UDF's for MySQL.

	Copyright: (c) 1998--2000 Jan Pazdziora, adelton@fi.muni.cz. All
		rights reserved. This package is free software; you can
		redistribute it and/or modify it under the terms of
		either GPL or Artistic Licence, whichever you like better.

	$VERSION = 0.97;
*/

#include <stdio.h>	/* we need NULL */
#include <string.h>

#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>

my_bool il1_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void il1_to_ascii_deinit(UDF_INIT *initid);
char *il1_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error);

my_bool il2_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void il2_to_ascii_deinit(UDF_INIT *initid);
char *il2_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error);

#ifndef NOWINSET
my_bool cp1250_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void cp1250_to_ascii_deinit(UDF_INIT *initid);
char *cp1250_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error);
my_bool cp1250_to_il2_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void cp1250_to_il2_deinit(UDF_INIT *initid);
char *cp1250_to_il2(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error);
my_bool il2_to_cp1250_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void il2_to_cp1250_deinit(UDF_INIT *initid);
char *il2_to_cp1250(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error);
#endif /* NOWINSET */

my_bool udf_charsets_convert_init(UDF_INIT *initid, UDF_ARGS *args,
						char *message) {
	if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT) {
		strcpy(message, "One string argument required for charset conversion");
		return 1;
	}
	initid->max_length = args->lengths[0];
	if (args->lengths[0] > 255) {
		initid->ptr = (void *)my_malloc(args->lengths[0], 0);
	} else {
		initid->ptr = NULL;
	}
	return 0;
}
int udf_charsets_convert(UDF_INIT *initid, UDF_ARGS *args, char *table,
							char *result) {
	char * ptr;
	int count = 0;
	if (args->args[0] == NULL) {
		return -1;
	}
	if (initid->ptr) {
		ptr = initid->ptr;
	} else {
		ptr = result;
	}
	while (count < args->lengths[0]) {
		ptr[count] = table[(unsigned char)args->args[0][count]];
		count++;
	}
	return count;
}
char * udf_charsets_convert_deinit(UDF_INIT *initid) {
	if (initid->ptr) my_free(initid->ptr, 0);
	return NULL;
}

my_bool il1_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return udf_charsets_convert_init(initid, args, message);
}
my_bool il2_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return udf_charsets_convert_init(initid, args, message);
}
#ifndef NOWINSET
my_bool cp1250_to_ascii_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return udf_charsets_convert_init(initid, args, message);
}
my_bool cp1250_to_il2_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return udf_charsets_convert_init(initid, args, message);
}
my_bool il2_to_cp1250_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
	return udf_charsets_convert_init(initid, args, message);
}
#endif /* NOWINSET */

void il1_to_ascii_deinit(UDF_INIT *initid) {
	udf_charsets_convert_deinit(initid);
}
void il2_to_ascii_deinit(UDF_INIT *initid) {
	udf_charsets_convert_deinit(initid);
}
#ifndef NOWINSET
void cp1250_to_ascii_deinit(UDF_INIT *initid) {
	udf_charsets_convert_deinit(initid);
}
void cp1250_to_il2_deinit(UDF_INIT *initid) {
	udf_charsets_convert_deinit(initid);
}
void il2_to_cp1250_deinit(UDF_INIT *initid) {
	udf_charsets_convert_deinit(initid);
}
#endif /* NOWINSET */

/*
	Put here output of ./make_il_to_ascii il1, ./make_il_to_ascii il2,
	./make_il_to_ascii cp1250, and ./make_il_to_ascii il2 cp1250
	and ./make_il_to_ascii cp1250 il2
*/

static char il1_to_ascii_conv_table[256] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', 
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', 
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027', 
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037', 
	' ', '!', '"', '#', '$', '%', '&', '\'', 
	'(', ')', '*', '+', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ':', ';', '<', '=', '>', '?', 
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
	'x', 'y', 'z', '{', '|', '}', '~', '', 
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207', 
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217', 
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227', 
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237', 
	' ', '!', 'c', 'L', ' ', 'y', '|', ' ', 
	'"', '(', ' ', '>', 'n', '-', '(', ' ', 
	' ', '+', ' ', ' ', '\'', 'u', ' ', '.', 
	' ', '\271', ' ', '<', '1', '1', '3', '?', 
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 
	'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I', 
	'D', 'N', 'O', 'O', 'O', 'O', 'O', '.', 
	'O', 'U', 'U', 'U', 'U', 'Y', ' ', 's', 
	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c', 
	'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 
	'd', 'n', 'o', 'o', 'o', 'o', 'o', '/', 
	'o', 'u', 'u', 'u', 'u', 'y', ' ', 'y', 
};

static char il2_to_ascii_conv_table[256] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', 
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', 
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027', 
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037', 
	' ', '!', '"', '#', '$', '%', '&', '\'', 
	'(', ')', '*', '+', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ':', ';', '<', '=', '>', '?', 
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
	'x', 'y', 'z', '{', '|', '}', '~', '', 
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207', 
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217', 
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227', 
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237', 
	' ', 'A', ' ', 'L', ' ', 'L', 'S', ' ', 
	'"', 'S', 'S', 'T', 'Z', '-', 'Z', 'Z', 
	' ', 'a', ' ', 'l', '\'', 'l', 's', ' ', 
	' ', 's', 's', 't', 'z', '"', 'z', 'z', 
	'R', 'A', 'A', 'A', 'A', 'L', 'C', 'C', 
	'C', 'E', 'E', 'E', 'E', 'I', 'I', 'D', 
	'D', 'N', 'N', 'O', 'O', 'O', 'O', '.', 
	'R', 'U', 'U', 'U', 'U', 'Y', 'T', 's', 
	'r', 'a', 'a', 'a', 'a', 'l', 'c', 'c', 
	'c', 'e', 'e', 'e', 'e', 'i', 'i', 'd', 
	'd', 'n', 'n', 'o', 'o', 'o', 'o', '/', 
	'r', 'u', 'u', 'u', 'u', 'y', 't', ' ', 
};

#ifndef NOWINSET
static char cp1250_to_ascii_conv_table[256] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', 
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', 
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027', 
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037', 
	' ', '!', '"', '#', '$', '%', '&', '\'', 
	'(', ')', '*', '+', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ':', ';', '<', '=', '>', '?', 
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
	'x', 'y', 'z', '{', '|', '}', '~', '\177', 
	'\200', '\201', '\'', '\203', '"', '.', '|', '|', 
	'\210', 'o', 'S', '<', 'S', 'T', 'Z', 'Z', 
	'\220', '`', '\'', '"', '"', '*', '-', '-', 
	'\230', '(', 's', '>', 's', 't', 'z', 'z', 
	' ', ' ', ' ', 'L', ' ', 'A', '|', ' ', 
	'"', '(', 'S', '>', 'n', '-', '(', 'Z', 
	' ', '+', ' ', 'l', '\'', 'u', '\266', ' ', 
	' ', 'a', 's', '<', 'L', '"', 'l', 'z', 
	'R', 'A', 'A', 'A', 'A', 'L', 'C', 'C', 
	'C', 'E', 'E', 'E', 'E', 'I', 'I', 'D', 
	'D', 'N', 'N', 'O', 'O', 'O', 'O', '.', 
	'R', 'U', 'U', 'U', 'U', 'Y', 'T', 's', 
	'r', 'a', 'a', 'a', 'a', 'l', 'c', 'c', 
	'c', 'e', 'e', 'e', 'e', 'i', 'i', 'd', 
	'd', 'n', 'n', 'o', 'o', 'o', 'o', '/', 
	'r', 'u', 'u', 'u', 'u', 'y', 't', ' ', 
};
static char il2_to_cp1250_conv_table[256] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', 
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', 
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027', 
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037', 
	' ', '!', '"', '#', '$', '%', '&', '\'', 
	'(', ')', '*', '+', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ':', ';', '<', '=', '>', '?', 
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
	'x', 'y', 'z', '{', '|', '}', '~', '\177', 
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207', 
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217', 
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227', 
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237', 
	'\240', '\245', '\242', '\243', '\244', '\274', '\214', '\247', 
	'\250', '\212', '\252', '\215', '\217', '\255', '\216', '\257', 
	'\260', '\271', '\262', '\263', '\264', '\276', '\234', '\241', 
	'\270', '\232', '\272', '\235', '\237', '\275', '\236', '\277', 
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307', 
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317', 
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327', 
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337', 
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347', 
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357', 
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367', 
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\267', 
	};
static char cp1250_to_il2_conv_table[256] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007', 
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017', 
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027', 
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037', 
	' ', '!', '"', '#', '$', '%', '&', '\'', 
	'(', ')', '*', '+', ',', '-', '.', '/', 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ':', ';', '<', '=', '>', '?', 
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_', 
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 
	'x', 'y', 'z', '{', '|', '}', '~', '\177', 
	'\200', '\201', '\'', '\203', '"', ' ', '|', '|', 
	'\210', ' ', '\251', '<', '\246', '\253', '\256', '\254', 
	'\220', '`', '\'', '"', '"', '*', ' ', ' ', 
	'\230', ' ', '\271', '>', '\266', '\273', '\276', '\274', 
	'\240', '\267', '\242', '\243', '\244', '\241', '|', '\247', 
	'\250', ' ', '\252', ' ', ' ', '\255', ' ', '\257', 
	'\260', ' ', '\262', '\263', '\264', 'u', '\266', '\377', 
	'\270', '\261', '\272', ' ', '\245', '\275', '\265', '\277', 
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307', 
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317', 
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327', 
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337', 
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347', 
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357', 
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367', 
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377', 
	};
#endif /* NOWINSET */

/* Here ends section produced as output of make_il_to_ascii */

char *il1_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error) {
	int len = udf_charsets_convert(initid, args, il1_to_ascii_conv_table,									result);
	if (len < 0) {
		*is_null = 1; return NULL;
	} else {
		*length = len;
	}
	return (initid->ptr ? initid->ptr : result);
}

char *il2_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error) {
	int len = udf_charsets_convert(initid, args, il2_to_ascii_conv_table,
								result);
	if (len < 0) {
		*is_null = 1; return NULL;
	} else {
		*length = len;
	}
	return (initid->ptr ? initid->ptr : result);
}

#ifndef NOWINSET
char *cp1250_to_ascii(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error) {
	int len = udf_charsets_convert(initid, args,
					cp1250_to_ascii_conv_table, result);
	if (len < 0) {
		*is_null = 1; return NULL;
	} else {
		*length = len;
	}
	return (initid->ptr ? initid->ptr : result);
}
char *cp1250_to_il2(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error) {
	int len = udf_charsets_convert(initid, args,
					cp1250_to_il2_conv_table, result);
	if (len < 0) {
		*is_null = 1; return NULL;
	} else {
		*length = len;
	}
	return (initid->ptr ? initid->ptr : result);
}
char *il2_to_cp1250(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error) {
	int len = udf_charsets_convert(initid, args,
					il2_to_cp1250_conv_table, result);
	if (len < 0) {
		*is_null = 1; return NULL;
	} else {
		*length = len;
	}
	return (initid->ptr ? initid->ptr : result);
}

#endif /* NOWINSET */

/*
The il[12]_to_ascii_conv_table was produced using this script make_il_to_ascii:

#!/usr/bin/perl -w

use strict;

use Cz::Cstocs;

if (@ARGV != 1) {
	die "Usage: make_il_to_ascii charset\n\twhere charset is input charset in cstocs notation (il2, il1)\n";
}

my $input = shift;
my $conv = new Cz::Cstocs($input, "ascii", "one_by_one" => 1);

print "static char ${input}_to_ascii_conv_table[256] = {\n\t";

for (0 .. 255) {
	my $out = &$conv(chr($_));
	$out = substr $out, 0, 1;
	if (not $out =~ /^[\040-\176]$/) {
		printf "'\\%03o', ", ord($out);
	} elsif ($out eq "'") {
		print "'\\'', ";
	} elsif ($out eq "\\") {
		print "'\\\\', ";
	} else {
		print "'$out', ";
	}
	print "\n\t" unless (($_ + 1) % 8);
}

print "};\n";
__END__

*/

