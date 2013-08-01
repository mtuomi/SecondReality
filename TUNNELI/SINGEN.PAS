var   x     : word;
      sinit : array[0..4096] of word;
      cosit : array[0..2048] of word;
      f     : file;
begin
    for x := 0 to 4096 do Sinit[x] := round(sin((x)/128*pi)*((x*3) div 128));
    for x := 0 to 2048 do Cosit[x] := round(cos((x)/128*pi)*((x*4) div 64));

    assign(f,'sinit.dat');
    rewrite(f,2);
    blockwrite(f,sinit,4097);
    blockwrite(f,cosit,2049);
    close(f);
end.