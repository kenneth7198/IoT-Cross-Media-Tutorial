param(
  [ValidateSet('pub','sub')][string]$Action = 'sub',
  [string]$BrokerHost = 'localhost',
  [int]$Port = 1883,
  [string]$Topic = 'test/topic',
  [string]$Message = 'hello'
)

function Test-Tool($name){
  $p = Get-Command $name -ErrorAction SilentlyContinue
  if (-not $p) { Write-Host "找不到 $name，請先安裝 Mosquitto 客戶端工具。https://mosquitto.org/download/" -ForegroundColor Yellow; exit 1 }
}

if ($Action -eq 'sub'){
  Test-Tool 'mosquitto_sub'
  Write-Host ("訂閱 {0} @ {1}:{2} (Ctrl+C 結束)" -f $Topic, $BrokerHost, $Port) -ForegroundColor Cyan
  mosquitto_sub -h $BrokerHost -p $Port -t $Topic -v
}
elseif ($Action -eq 'pub'){
  Test-Tool 'mosquitto_pub'
  Write-Host ("發布到 {0} @ {1}:{2} => {3}" -f $Topic, $BrokerHost, $Port, $Message) -ForegroundColor Green
  mosquitto_pub -h $BrokerHost -p $Port -t $Topic -m $Message
}
