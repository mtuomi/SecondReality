uses crt;

type    pbuf = ^buf;
        buf = array[0..64000] of byte;


var    r,r1,nb,g,b  : pbuf;
       f      : file;
       x,y,c  : word;
       row    : array[0..320*3-1] of byte;
       tw     : word;
       tb     : word;
       tbuf   : array[0..100] of word;
       pal    : array[0..768] of byte;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

procedure tee(n1,n2,n3:string);
begin
     assign(f,n1);
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
     assign(f,n2);
     reset(f,1);
     seek(f,8);
     for y := 0 to 199 do
     begin
      blockread(f,row,320*3);
      for x := 0 to 319 do r1^[x+y*320] := row[x*3+0];
     end;
     close(f);
     assign(f,n3);
     rewrite(f,1);
     for y := 1 to 31 do
     for c := 4 to 240 do
     begin
       tw := 0;
       for x := 0 to 64000 do
         if (nb^[x] <> $40) and
            (r^[x] <> r1^[x]) and
            (r^[x] = c) and
            (r^[x+1] = c) and
            (g^[x] = y) and
            (b^[x] = 255) then
         begin
           tbuf[tw] := x;
           nb^[x] := $40;
           nb^[x+1] := $40;
           memw[$a000:x] := $4040;
           inc(tw);
         end;
       blockwrite(f,tw,2);
       if tw > 0 then blockwrite(f,tbuf,tw*2);

       tb := 0;
       for x := 0 to 64000 do
         if (nb^[x] <> $40) and
            (r^[x] <> r1^[x]) and
            (r^[x] = c) and
            (g^[x] = y) and
            (b^[x] = 255) then
         begin
           tbuf[tb] := x;
           nb^[x] := $40;
           mem[$a000:x] := $40;
           inc(tb);
         end;
       blockwrite(f,tb,2);
       if tb > 0 then blockwrite(f,tbuf,tb*2);
     end;
     close(f);
end;

begin
     getmem(r,64000);
     getmem(r1,64000);
     getmem(g,64000);
     getmem(b,64000);
     getmem(nb,64000);
     fillchar(r^,64000,#0);
     fillchar(r1^,64000,#0);
     fillchar(g^,64000,#0);
     fillchar(b^,64000,#0);
     fillchar(nb^,64000,#0);

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
     tee('o:mask1.mtv','o:mask2.mtv','pos1.dat');
     tee('o:mask2.mtv','o:mask3.mtv','pos2.dat');
     tee('o:mask3.mtv','o:mask4.mtv','pos3.dat');

     asm
     mov   ax,$3
     int   $10
     end;

     writeln('Homma on valmis !!!!!!!!!!!!!!!');

     freemem(r,64000);
     freemem(r1,64000);
     freemem(g,64000);
     freemem(b,64000);
     freemem(nb,64000);
end.