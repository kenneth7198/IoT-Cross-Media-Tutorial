# 簡易靜態伺服器：用 PowerShell 在 3000 埠提供本目錄
param([int]$Port=3000)

Add-Type -AssemblyName System.Web
$listener = New-Object System.Net.HttpListener
$prefix = "http://+:$Port/"
$listener.Prefixes.Add($prefix)
$listener.Start()
Write-Host "Serving $(Get-Location) at $prefix (Ctrl+C to stop)"

try {
  while ($listener.IsListening) {
    $context = $listener.GetContext()
    $path = $context.Request.Url.AbsolutePath.Trim('/')
    if ([string]::IsNullOrEmpty($path)) { $path = 'index.html' }
    $file = Join-Path (Get-Location) $path
    if (-not (Test-Path $file)) { $context.Response.StatusCode = 404; $context.Response.Close(); continue }
    $bytes = [System.IO.File]::ReadAllBytes($file)
    $context.Response.ContentType = [System.Web.MimeMapping]::GetMimeMapping($file)
    $context.Response.OutputStream.Write($bytes, 0, $bytes.Length)
    $context.Response.Close()
  }
} finally {
  $listener.Stop()
}
