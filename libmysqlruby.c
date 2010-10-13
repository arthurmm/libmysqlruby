/**
 * mapaBRASIL.net
 *
 *
 * Interpretador do Ruby dentro do mySQL
 * data 20080425
 */

#include"mysql.h"
#include"ruby.h"
#include<string.h>
#include<stdio.h>
#include<stddef.h>

#define HELP ("irb (Interpretador de ruby)\n" \
             "Sintaxe: irb([argumentos,...], 'script em ruby|PATH completo e absoluto de um script ruby')\n" \
             "Argumentos: todos os serão transformados em STRING\n" \
             "Os argumentos estarao no array global $ARGSQLV\n" \
             "No final do script, deve-se retornar o valor para a variável global $OUTPUT, esta variavel é STRING\n" \
             "Exemplo: select irb(2, 2, '$OUTPUT=($ARGSQLV[0].to_i+$ARGSQLV[1].to_i')).to_s")

my_bool irb_init(UDF_INIT *initid, UDF_ARGS *args, char *message);

void irb_deinit(UDF_INIT *initid);

char *irb(UDF_INIT * initid, UDF_ARGS * args, char *result, unsigned long *length, char *is_null, char *error);


my_bool irb_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    char *buffer=0, command[255];
    FILE *tmp=0;
    int i=0;
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb_init: inicio\n");
    #endif
    
    if(args->arg_count < 1) {
            strmov(message, HELP);
            return 1;
    }
    
    initid->maybe_null=0;
    initid->const_item=0;
    initid->max_length=255;
    
    for(i=0; i<args->arg_count-1; i++)
        if(args->arg_type[0]==INT_RESULT)
            args->arg_type[0]=STRING_RESULT;
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb_init: cast todos os parâmetros para string\n");
    #endif
    
    ruby_init();
    ruby_init_loadpath();
    ruby_script("embedded");
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb_init: inicia ruby\n");
    #endif
    
    buffer=malloc(255);

    initid->ptr=(void*) buffer;
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb_init: malloc(buffer)\n");
    #endif
    
    /* copia o ultimo campo passado como parametro e verifica se e uma rotina */
    memcpy(command, (char*) args->args[args->arg_count-1], (size_t) args->lengths[args->arg_count-1]);
    command[args->lengths[args->arg_count-1]]=0;
    if((tmp=fopen(command, "r"))!=0) {
        buffer[0]='F';
        #ifdef _DEBUG_
        fprintf(stdout, "irb_init: verifica se é um .rb %x %s\n", tmp, command);
        #endif
        fclose(tmp);
    }
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb_init: saida\n");
    #endif
    
    return 0;
}

void irb_deinit(UDF_INIT *initid) {
	ruby_finalize();
	free(initid->ptr);
}

char *irb(UDF_INIT * initid, UDF_ARGS * args, char *result, unsigned long *length, char *is_null, char *error) {
    char *buffer=0, command[255];
    int j=0, i=0;
    VALUE in=0;
    VALUE *output=0;
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb: inicio\n");
    #endif
    
    command[0]=0;
    buffer=(char*) initid->ptr;

    *is_null=0;
    *error=0;
    *length=0;
    
    /* definindo variavel de saida */
    rb_define_variable("$OUTPUT", (VALUE*) &output);
    #ifdef _DEBUG_
    fprintf(stdout, "irb: define $OUTPUT\n");
    #endif
    
    /* cria objeto Array com todos as variaveis passadas como parametro */
    in=rb_ary_new();
    #ifdef _DEBUG_
    fprintf(stdout, "irb: inicializa ARGSQLV (j=%d, arg_count=%d)\n", j, args->arg_count);
    #endif
    for(j=0; j<=args->arg_count-2; j++) {
        memccpy(command, (char*) args->args[j], (size_t) initid->max_length, (size_t) args->lengths[j]);
        #ifdef _DEBUG_
        fprintf(stdout, "irb: memcpy (coletando variaveis para gerar ARGSQLV)\n => j=%d, ", j);
        for(i=0; i<args->lengths[j]; i++) {
            fputc(args->args[j][i], stdout);
        }
        fprintf(stdout, "\n");
        #endif

        command[args->lengths[j]]=0;
        #ifdef _DEBUG_
        fprintf(stdout, "irb: command[args->lengths[j]]=0 (coletando variaveis para gerar ARGSQLV) => %d, %s\n", j, command);
        #endif

        rb_ary_push(in, rb_str_new2(command));
        #ifdef _DEBUG_
        fprintf(stdout, "irb: rb_ary_push (coletando variaveis para gerar ARGSQLV)\n");
        #endif
    }
    rb_define_variable("$ARGSQLV", (VALUE*) &in);
    #ifdef _DEBUG_
    fprintf(stdout, "irb: define $ARGSQLV\n");
    #endif
    /**/
    
    /* copia o ultimo campo passado como parametro e verifica se e uma rotina */
    j=args->arg_count-1;
    memcpy(command, (char*) args->args[j], (size_t) args->lengths[j]);
    command[args->lengths[j]]=0;
    
    /* setando $OUTPUT como STDOUT e definindo STDOUT para /dev/null */
    //rb_eval_string("$tmp=STDOUT.dup; STDOUT.reopen('/home/files/irb.mysql.txt', 'w')");
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb: copia o commando para j=%d, command=%s|%d, buffer[0]=%c\n", j, command, args->lengths[j], buffer[0]);
    #endif
    
    /* verifica se o que foi passado como parametro e uma rotina ou um arquivo .rb */
    if(buffer[0]=='F') {
        rb_load_file(command);
        ruby_exec();
        #ifdef _DEBUG_
        fprintf(stdout, "irb: ruby_exec\n");
        #endif
    }
    else {
        rb_eval_string(command);
        #ifdef _DEBUG_
        fprintf(stdout, "irb: rb_eval_string\n");
        #endif
    }
    
    /*
    rb_eval_string("$OUTPUT=File.open('/home/files/irb.mysql.txt').readlines.join(" "); STDOUT.reopen($tmp)");
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb: recuperando valores de saida\n");
    #endif
    */
    strcpy((char*) buffer, (char*) STR2CSTR(output));
    
    *length=strlen(buffer);
    if(*length > initid->max_length) {
        *length=initid->max_length;
    }
    
    #ifdef _DEBUG_
    fprintf(stdout, "irb: fim\n");
    #endif
    
    memcpy((char*) result, (char*) buffer, (size_t) *length);

    return result;
}
