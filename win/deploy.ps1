# Only pack when changes are made
$target    = $args[0]
$lastBuild = "$target.lastbuild"

if (Test-Path $lastBuild) {
    $oldHash = Get-FileHash $lastBuild | Select-Object -ExpandProperty Hash
    $newHash = Get-FileHash $target    | Select-Object -ExpandProperty Hash
    
    if ($oldHash -eq $newHash) {
        exit 0
    }
}

Copy-Item $target $lastBuild -Force

# Packing
$nuspecFile = Get-ChildItem -Filter "*.nuspec" | Select-Object -First 1
$newVersion = $([System.DateTime]::Now.ToString("yyyy.M.d.Hmm"))

if ($nuspecFile) {
    (Get-Content $nuspecFile.FullName) -replace '(<version>).*?(</version>)', "`$1 $newVersion `$2" | Set-Content $nuspecFile.FullName
    Write-Host "Updated version to $newVersion in $($nuspecFile.Name)"
	rm    *.nupkg
	nuget pack
	nuget push *.nupkg -Source GitHub
} else {
    Write-Host "No .nuspec file found."
}
