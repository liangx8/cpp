#include <alsa/asoundlib.h>
#include <string.h>
#include <stdio.h>

#define COLUMN 19
void pair(const char *s1,const char *s2)
{
    int len=0;
    while(*s1){
        putchar(*s1);
        s1++;
        len++;
    }
    for(int ix=len;ix<COLUMN;ix++){
        putchar(' ');
    }
    puts(s2);
}
void snd_list(void)
{
    int val;
    printf("ALSA library version %s\n", SND_LIB_VERSION_STR);
    printf("PCM stream types:\n");
    for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
    {
        printf("%d:%s\n", val, snd_pcm_stream_name((snd_pcm_stream_t)val));
    }
    printf("pcm access types:\n");
    for (val = 0; val < SND_PCM_ACCESS_LAST; val++)
    {
        printf("%d: %s\n", val, snd_pcm_access_name((snd_pcm_access_t)val));
    }
    printf("\nPCM formats:\n");
    for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
    {
        const char *name = snd_pcm_format_name((snd_pcm_format_t)val);
        if (name != NULL){
            printf("%2d:",val);
            pair(name,snd_pcm_format_description((snd_pcm_format_t)val));

        }
    }
    printf("PCM subformats\n");
    for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++){
        printf("%2d:",val);
        pair(snd_pcm_subformat_name((snd_pcm_subformat_t)val),snd_pcm_subformat_description((snd_pcm_subformat_t)val));
    }
    printf("PCM states\n");
    for (val = 0; val <= SND_PCM_STATE_LAST; val++)
        printf("%2d,%s\n", val, snd_pcm_state_name((snd_pcm_state_t)val));
}