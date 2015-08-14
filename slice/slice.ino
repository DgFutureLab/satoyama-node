const int tx_size = 20;

void setup(){
  Serial.begin(115200);
  char s[] = "vbat:2.00:16152;temperature:28.00:16175;humidity:39.00:16175;vbat:2.00:16175;temperature:28.00:16236;humidity:39.00:16236;vbat:2.00:16237;temperature:28.00:16297;humidity:39.00:16299;vbat:2.00:16299;temperature:28.00:16321;humidity:43.00:16321;vbat:2.00:16321;temperature:28.00:16382;humidity:43.00:16382;";
  int l = strlen(s);
  Serial.println(l);
//  for(int i = 0; i<5; i++){
  
  int chunks = floor(l/tx_size);
  int remaining = l - chunks*tx_size;
  Serial.println(l);
  Serial.println(chunks);
  Serial.println(remaining);
  char buf[tx_size+1];

  for(int chunk = 0; chunk < chunks; chunk++){
    memset(buf, 0, tx_size);
    memcpy(buf, &s[chunk * tx_size], tx_size);
    buf[tx_size] = '\0';
    Serial.println(buf);
  }
  memset(buf, 0, tx_size);
  memcpy(buf, &s[chunks * tx_size], remaining);
  buf[remaining] = '\0';
  Serial.println(buf);
  Serial.println(strlen(buf));
  
}
void loop(){

}
