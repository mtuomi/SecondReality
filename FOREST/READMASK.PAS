uses crt;

type    pbuf = ^buf;
        buf = array[0..64000] of byte;


var    r,r1,
       g,b  : pbuf;
       f      : file;
       x,y,c  : word;
       row    : array[0..320*3-1] of byte;
       tc     : word;
       tbuf   : array[0..100] of word;
       pal    : array[0..768] of byte;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

begin
     getmem(r,64000);
     getmem(r1,64000);
     getmem(g,64000);
     getmem(b,64000);
     fillchar(r^,64000,#0);
     fillchar(r1^,64000,#0);
     fillchar(g^,64000,#0);
     fillchar(b^,64000,#0);

     { Load fixed green mask }
     assign(f,'green.clx');
     reset(f,1);
     seek(f,778);
     blockread(f,g^,64000);
     close(f);

     asm
     mov   ax,$13
     int   $10
     end;
{
     assign(f,'hillback.clx');
     reset(f,1);
     seek(f,10);
     blockread(f,pal,768);
     for x := 0 to 255 do setrgb(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);
     blockread(f,mem[$a000:0],64000);
     close(f);
}

     assign(f,'o:mask1.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do
       begin
        r^[x+y*320] := row[x*3+0];
        b^[x+y*320] := row[x*3+2];
       end;
     end;
     close(f);
     assign(f,'o:mask2.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r1^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,'pos1.dat');
     rewrite(f,1);
     { red = 4 to 240 }
     for y := 1 to 31 do
     for c := 4 to 240 do
     begin
       tc := 0;
       for x := 0 to 64000 do
         if (mem[$a000:x] <> 40) and (r^[x] <> r1^[x]) and (r^[x] = c) and (g^[x] = y) and (b^[x] = 255) then
         begin
           tbuf[tc] := x;
           mem[$a000:x] := 40;
           inc(tc);
         end;
       blockwrite(f,tc,2);
       if tc > 0 then blockwrite(f,tbuf,tc*2);
     end;
     close(f);


     assign(f,'o:mask2.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do
       begin
        r^[x+y*320] := row[x*3+0];
        b^[x+y*320] := row[x*3+2];
       end;
     end;
     close(f);
     assign(f,'o:mask3.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r1^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,'pos2.dat');
     rewrite(f,1);
     { red = 4 to 240 }
     for y := 1 to 31 do
     for c := 4 to 240 do
     begin
       tc := 0;
       for x := 0 to 64000 do
         if (mem[$a000:x] <> 40) and (r^[x] <> r1^[x]) and (r^[x] = c) and (g^[x] = y) and (b^[x] = 255) then
         begin
           tbuf[tc] := x;
           mem[$a000:x] := 40;
           inc(tc);
         end;
       blockwrite(f,tc,2);
       if tc > 0 then blockwrite(f,tbuf,tc*2);
     end;
     close(f);


     assign(f,'o:mask3.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do
       begin
        r^[x+y*320] := row[x*3+0];
        b^[x+y*320] := row[x*3+2];
       end;
     end;
     close(f);
     assign(f,'o:mask4.mtv');
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r1^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,'pos3.dat');
     rewrite(f,1);
     { red = 4 to 240 }
     for y := 1 to 31 do
     for c := 4 to 240 do
     begin
       tc := 0;
       for x := 0 to 64000 do
         if (mem[$a000:x] <> 40) and (r^[x] <> r1^[x]) and (r^[x] = c) and (g^[x] = y) and (b^[x] = 255) then
         begin
           tbuf[tc] := x;
           mem[$a000:x] := 40;
           inc(tc);
         end;
       blockwrite(f,tc,2);
       if tc > 0 then blockwrite(f,tbuf,tc*2);
     end;
     close(f);



     repeat until keypressed;

     asm
     mov   ax,$3
     int   $10
     end;

     freemem(r,64000);
     freemem(r1,64000);
     freemem(g,64000);
     freemem(b,64000);
end.