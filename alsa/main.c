/*
     https://blog.51cto.com/buaadallas/403832
     
 */
#include <alsa/asoundlib.h>

int main(int argc,char **argv)
{
  int pcm_idx;
  printf("ALSA library version %s\n",SND_LIB_VERSION_STR);
  printf("pcm stream list:\n");
  for(pcm_idx=0;pcm_idx<SND_PCM_STREAM_LAST;pcm_idx++){
    printf("%d: %s\n",pcm_idx,snd_pcm_stream_name((snd_pcm_stream_t)pcm_idx));
  }
  printf("pcm access list:\n");
  for(pcm_idx=0;pcm_idx<SND_PCM_ACCESS_LAST;pcm_idx++){
    printf("%d: %s\n",pcm_idx,snd_pcm_access_name((snd_pcm_access_t)pcm_idx));
  }
}
